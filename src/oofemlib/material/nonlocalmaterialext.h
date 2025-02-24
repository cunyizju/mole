// Originated from OOFEM (https://github.com/oofem/oofem)
// License: GNU Lesser General Public
// Modified by CY Li

#ifndef nonlocalmaterialext_h
#define nonlocalmaterialext_h

#include "material/matstatus.h"
#include "utility/interface.h"
#include "math/intarray.h"
#include "math/grid.h"
#include "math/mathfem.h"
#include "input/dynamicinputrecord.h"

#include <list>
#include <memory>
#ifdef _OPENMP
#include <omp.h>
#endif
///@name Input fields for NonlocalMaterialExtensionInterface
//@{
#define _IFT_NonlocalMaterialExtensionInterface_regionmap "regionmap"
#define _IFT_NonlocalMaterialExtensionInterface_permanentNonlocTableFlag "permanentnonloctableflag"
#define _IFT_NonlocalMaterialExtensionInterface_r "r"
#define _IFT_NonlocalMaterialExtensionInterface_wft "wft"
#define _IFT_NonlocalMaterialExtensionInterface_averagingtype "averagingtype"
#define _IFT_NonlocalMaterialExtensionInterface_m "m"
#define _IFT_NonlocalMaterialExtensionInterface_scalingtype "scaling"
#define _IFT_NonlocalMaterialExtensionInterface_averagedquantity "averagedvar"
#define _IFT_NonlocalMaterialExtensionInterface_nonlocalvariation "nlvariation"
#define _IFT_NonlocalMaterialExtensionInterface_beta "beta"
#define _IFT_NonlocalMaterialExtensionInterface_zeta "zeta"
#define _IFT_NonlocalMaterialExtensionInterface_px "px"
#define _IFT_NonlocalMaterialExtensionInterface_averagingtype "averagingtype"
#define _IFT_NonlocalMaterialExtensionInterface_exp "exp"
#define _IFT_NonlocalMaterialExtensionInterface_rf "rf"
#define _IFT_NonlocalMaterialExtensionInterface_gridsize "gridsize"
#define _IFT_NonlocalMaterialExtensionInterface_initdiag "initdiag"
#define _IFT_NonlocalMaterialExtensionInterface_order "order"
#define _IFT_NonlocalMaterialExtensionInterface_centdiff "centdiff"
//@}

namespace oofem {
/**
 * Structure containing reference to integration point and its corresponding nonlocal integration weight.
 * Used by nonlocal constitutive models based on integral averaging procedure, where in each integration
 * point the corresponding list of influencing integration points is kept, together with their weights.
 * This structure encapsulates the reference to influencing integration point and its corresponding weight.
 */
struct localIntegrationRecord {
    /// Reference to influencing integration point.
    GaussPoint *nearGp;
    /// Corresponding integration weight.
    double weight;
};

  enum WeightFunctionType {
                           WFT_Unknown,
                           WFT_Bell,
                           WFT_Gauss,
                           WFT_Green,
                           WFT_Uniform,
                           WFT_UniformOverElement,
                           WFT_Green_21 }; 

/**
 * Abstract base class for all nonlocal constitutive model statuses. Introduces the list of
 * localIntegrationRecords stored in each integration point, where references to all influencing
 * integration points with their integration weights are kept. Also the total volume associated to
 * corresponding integration point is kept. Services for accessing of integration list as well as
 * services for manipulating and requesting integration volume are provided.
 * Generally speaking, the nonlocal weight function with "bounded" or limited support is assumed.
 * When nonlocal weight function unbounded support is used, then keeping the list of
 * influencing integration points has no sense (because all integration points in domain participate)
 * and should be appropriate not to use afore mentioned list and redefine the
 * MaterialStatus::buildNonlocalPointTable service to void service.
 *
 * @see localIntegrationRecord
 * @see MaterialStatus::buildNonlocalPointTable
 */
class OOFEM_EXPORT NonlocalMaterialStatusExtensionInterface : public Interface
{
protected:
    /// List containing localIntegrationRecord values.
    std :: vector< localIntegrationRecord >integrationDomainList;
    /// Nonlocal volume around the corresponding integration point.
    double integrationScale;
    /// Local volume around the corresponding integration point.
    double volumeAround;

public:
    /**
     * Constructor.
     */
    NonlocalMaterialStatusExtensionInterface();

    /// Destructor.
    virtual ~NonlocalMaterialStatusExtensionInterface();

    /**
     * Returns integration list of receiver. Contains localIntegrationRecord structures, containing
     * references to integration points and their weights that influence the nonlocal average in
     * receiver's associated integration point.
     */
    std :: vector< localIntegrationRecord > *giveIntegrationDomainList() { return & integrationDomainList; }
    /// Returns associated integration scale.
    double giveIntegrationScale() { return integrationScale; }
    /// Sets associated integration scale.
    void setIntegrationScale(double val) { integrationScale = val; }
    /// Returns associated volume.
    double giveVolumeAround() { return volumeAround; }
    /// Sets associated integration scale.
    void setVolumeAround(double val) { volumeAround = val; }
    /// clears the integration list of receiver
    void clear() { integrationDomainList.clear(); }
};


/**
 * Abstract base class for all nonlocal materials. Nonlocal in sense, that response in particular
 * point depends not only on state in that point, but also takes into account state of surrounding
 * points. Response typically depends on some nonlocal quantity obtained as nonlocal average over
 * some characteristic volume.
 * General services for updating domain before nonlocal average, building table of influencing
 * integration points for given integration point and computing nonlocal weight function are
 * declared. The general service for building table of influencing
 * integration points for given integration point is also implemented.
 * The use of multiple inheritance is assumed. Typically, the class representing nonlocal
 * constitutive model is derived both from class representing local model and from this class or
 * from one of its derived classes (which declare services and variables corresponding to specific
 * analysis type).
 */
class OOFEM_EXPORT NonlocalMaterialExtensionInterface : public Interface
{
protected:
    /*
     * It is necessary, mainly due to resulting efficiency, to compute variable(s)
     * which are nonlocally averaged in advance, before average process begins.
     * The loop over all  integration points is typically made to compute these variables.
     * To prevent doing this multiple times at the same solution state,
     * the modification time mark is kept.
     * In the present implementation, there is one common stateCounter for the whole domain.
     * This implies, that all variables (which undergo averaging) for all material models of the domain
     * should be prepared at the same time in updateDomainBeforeNonlocAverage method.
     * It is believed that this is general enough and can somehow handle even the case of multiple models
     * with different parameters being averaged (but is this realistic?).
     * If this scheme will not be enough general, then the state counter
     * can be kept as attribute of NonlocalMaterialExtensionInterface, so independently for
     * each material model. Each model will be then updated in separate call. But in the case of
     * several material models of the same type (with different parameters) this will lead to
     * multiple update, which can not be avoided, although it is redundant.
     *
     * StateCounterType lastUpdatedStateCounter;
     */
    Domain *domain = nullptr;
    /// Map indicating regions to skip (region - cross section model).
    IntArray regionMap;
    /// Flag indicating whether to keep nonlocal interaction tables of integration points cached.
    bool permanentNonlocTableFlag = false;
    /// Parameter specifying the type of nonlocal weight function.
    WeightFunctionType weightFun;
    /// Grid on which the eikonal equation will be solved (used by eikonal nonlocal models)
    int gridSize = 0;
    /// Optional parameters setting details of the fast marching method
    double initDiag = 0.;
    int order = 0;
    int centDiff = 0;

    /**
     * Characteristic length of the nonlocal model
     * (its interpretation depends on the type of weight function).
     */
    mutable double cl = 0.;

    /// Support radius.
    mutable double suprad = 0.;

    /// For "undernonlocal" or "overnonlocal" formulation.
    double mm = 0.;

    /// Type characterizing the scaling approach.
    enum ScalingType { ST_Unknown, ST_Standard, ST_Noscaling, ST_Borino };
    /// Parameter specifying the type of scaling of nonlocal weight function.
    ScalingType scaling;

    /// Type characterizing the averaged (nonlocal) variable.
    enum AveragedVarType { AVT_Unknown, AVT_EqStrain, AVT_Compliance, AVT_Damage };
    /// Parameter specifying the type of averaged (nonlocal) variable.
    AveragedVarType averagedVar;

    /**
     * Initial(user defined) characteristic length of the nonlocal model
     * (its interpretation depends on the weight function)
     * Is different to cl when a Stress-based or a Distance-based
     * nonlocal variation is applied
     */
    double cl0 = 0.;
    /// Type characterizing the Nonlocal variation
    enum NlVariationType { NLVT_Standard, NLVT_DistanceBasedLinear, NLVT_StressBased, NLVT_DistanceBasedExponential };
    ///Parameter specifying the type of nonlocal variation.
    NlVariationType nlvar;
    /**
     * Parameter which multiplied with the interaction radius cl0
     * gives its minimum allowed value. It is used when a Stress-based
     * or a Distance-based nonlocal variation is applied
     */
    double beta = 0.;
    /**
     * Parameter used when Distance-based nonlocal variation is applied
     * When it is multiplied with the interaction radius cl gives the maxinmum
     * distance of the Gauss Point from the boundary. If the Gauss Point's distance
     * from the boundary is larger than this value the interaction radius cl is set
     * to cl0
     */
    double zeta = 0.;

    /**
     * Parameter specifying the periodic shift in x-direction. Typically it is zero.
     * If it is set to a positive value, the nonlocal interaction is considered not only
     * if the distance between the receiver and the source is smaller than the interaction radius
     * but also if the source point shifted by -px or +px satisfies this condition.
     * This is useful if the mesh represents a periodic cell.
     */
    double px = 0.;

    // Parameters used by models with evolving characteristic length.

    /// Final value of interaction radius, for a model with evolving characteristic length.
    double Rf = 0.;
    /// Parameter used as an exponent by models with evolving characteristic length.
    double exponent = 0.;
    /// Parameter specifying how the weight function should be adjusted due to damage.
    int averType = 0;

#ifdef _OPENMP
 public:
    static omp_lock_t updateDomainBeforeNonlocAverageLock;
#endif
public:
    /**
     * Constructor. Creates material with given number, belonging to given domain.
     * @param d Domain to which new material will belong.
     */
    NonlocalMaterialExtensionInterface(Domain *d);
    /// Destructor.
    virtual ~NonlocalMaterialExtensionInterface() { }


    /**
     * Updates data in all integration points before nonlocal average takes place.
     * It is necessary, mainly due to resulting efficiency, to compute variable(s)
     * which are nonlocally averaged in advance, before average process begins.
     * These variables must be stored in integration point's associated statuses.
     * This function updates the whole problem domain, by updating all integration points
     * values, which take part in nonlocal average process. All elements are updated using
     * Element::updateBeforeNonlocalAverage abstract service, which in turn updates all
     * integration points associated with particular element.
     * The service used to update element integration point depends on analysis type and
     * is specified by element-specific type (like StructuralElement) corresponding to analysis type.
     * This service can be invoked multiple times, but update for specific material is done only once, because
     * last modification time mark is kept.
     * @see Element::updateBeforeNonlocalAverage
     */
    void updateDomainBeforeNonlocAverage(TimeStep *tStep) const;

    /**
     * Builds list of integration points which take part in nonlocal average in given integration point.
     * This list is stored in integration point corresponding nonlocal status.
     * Generally speaking, the nonlocal weight function with "bounded" or limited support is assumed.
     * When nonlocal weight function unbounded support is used, then keeping the list of
     * influencing integration points would be wasting of space and should be cleared after averaging has
     * been finished in integration point. The endIPNonlocalAverage method will ensure this.
     */
    void buildNonlocalPointTable(GaussPoint *gp) const;

    /**
     * Rebuild list of integration points which take part
     * in nonlocal average in given integration point.
     * if contributingElems param is not NULL, then it is assumed that it contains
     * a <em>complete</em> list of elements contributing to receiver. If equal to NULL
     * existing list is cleared and  buildNonlocalPointTable service is invoked.
     */
    void rebuildNonlocalPointTable(GaussPoint *gp, IntArray *contributingElems) const;

    /**
     * Recompute the nonlocal interaction weights based on the current solution (e.g., on the damage field).
     * This method is used e.g. by eikonal nonlocal damage models.
     */
    void modifyNonlocalWeightFunctionAround(GaussPoint *gp) const;
    void modifyNonlocalWeightFunction_1D_Around(GaussPoint *gp) const;

    /**
     * Provide the current value of the variable that affects nonlocal interaction (e.g., of damage)
     * This method is used e.g. by eikonal nonlocal damage models.
     */
    virtual double giveNonlocalMetricModifierAt(GaussPoint *gp) const { return 1.; }

    /**
     * Compute the factor that specifies how the interaction length should be modified,
     * based on the current solution (e.g., on the damage field).
     * This method is used e.g. by eikonal nonlocal damage models.
     */
    double computeDistanceModifier(double cl, double damage) const;

    /**
     * Compute the modified interaction length based on the current solution (e.g., on the damage field).
     * This method is used e.g. by eikonal nonlocal damage models.
     */
    double computeModifiedLength(double length, double dam1, double dam2) const;

    /**
     * Returns integration list corresponding to given integration point.
     * Contains localIntegrationRecord structures, containing
     * references to integration points and their weights that influence to nonlocal average in
     * receiver's associated integration point.
     * Rebuilds the IP list by calling  buildNonlocalPointTable if not available.
     */
    std :: vector< localIntegrationRecord > *giveIPIntegrationList(GaussPoint *gp) const;

    /**
     * Evaluates the basic nonlocal weight function for a given distance
     * between interacting points. This function is NOT normalized by the
     * condition of unit integral.
     * @param distance Distance between interacting points.
     * @return Value of weight function.
     */
    virtual double computeWeightFunction(const double cl, const double distance) const;

    /**
     * Evaluates the basic nonlocal weight function for two points
     * with given coordinates. This function is NOT normalized by the
     * condition of unit integral.
     * @param src Coordinates of source point.
     * @param coord Coordinates of receiver point.
     * @return Value of weight function.
     */
    virtual double computeWeightFunction(const double cl, const FloatArray &src, const FloatArray &coord) const;

    /**
     * Provides the integral of the weight function
     * over the contributing volume in 1, 2 or 3D.
     */
    double giveIntegralOfWeightFunction(double cl, const int spatial_dimension) const;


    /// Determines the maximum value of the nonlocal weight function.
    virtual double maxValueOfWeightFunction();

    /**
     * Determines the number of material regions of domain.
     * In the current implementation the region is associated with cross section model.
     */
    int giveNumberOfRegions() const;
    /*
     * Returns the region id of given element
     * @param element pointer to element which region id is requested.
     * @return Region id (number) for this element.
     */
    //int giveElementRegion (Element* element);
    /**
     * Determines, whether receiver has bounded weighting function (limited support).
     * @return True if weighting function bounded, zero otherwise.
     */
    virtual int hasBoundedSupport() const { return 1; }
    /**
     * Determines the width (radius) of limited support of weighting function.
     * i.e., the distance at which the interaction weight becomes zero.
     */
    virtual double evaluateSupportRadius(double cl) const;

    /// Returns reference to domain.
    Domain *giveDomain() { return this->domain; }

    void initializeFrom(InputRecord &ir);
    /**
     * Stores receiver in an input record
     * @param input Input record to be filled in.
     */
    void giveInputRecord(DynamicInputRecord &input);
    /*
     * Creates new copy of associated status and inserts it into given integration point.
     * @param gp Integration point where newly created status will be stored.
     * @return reference to new status.
     */
    //MaterialStatus* CreateStatus (GaussPoint* gp)
    // {return  new NonlocalMaterialStatus (1,this->giveDomain(), gp);;}
    /**
     * Notifies the receiver, that the nonlocal averaging has been finished for given ip.
     * It deletes IP nonlocal table if permanentNonlocTableFlag is false.
     * This can save significant memory, since nonlocal tables are not stored, but every time computed when needed,
     * but on the other hand computational time may significantly grow.
     */
    void endIPNonlocalAverage(GaussPoint *gp) const;

protected:
    /*
     * Returns true if the barrier is activated
     * by interaction of two given points. In this case the nonlocal influence
     * is not considered.
     * @param gpCoords Coordinates of first point.
     * @param jGpCoords Coordinates of second point.
     * @param weight Set to zero if the GP are across the barrier.
     */
    //bool isBarrierActivated (const FloatArray& c1, const FloatArray& c2) const;

    void applyBarrierConstraints(const FloatArray &gpCoords, const FloatArray &jGpCoords, double &weight) const;

    /**
     * Manipulates weight on integration point in the element.
     * By default is off, keyword 'averagingtype' specifies various methods.
     * For example, a boundary layer method averages strains over the whole element without any radius.
     * @param weight Modifies the weight with the jGp-th item.
     * @param gp Pointer to the GP owing the PointTable.
     * @param jGp Pointer to GP in the PointTable.
     */
    void manipulateWeight(double &weight, GaussPoint *gp, GaussPoint *jGp) const;

    /**
     * Provides the distance based interaction radius
     * This function is called when nlvariation is set to 1.
     * The function loops over all user defined nonlocal boundaries to
     * to find minimum distance from the GP. Then calculates interaction radius
     * @param gpCoords The Gauss points' coordinates, whose interaction radius is calculated based on the distance-based averaging approach.
     * @return New interaction radius based on the Distance of the Gauss Point from Nonlocal Boundaries.
     */
    double giveDistanceBasedInteractionRadius(const FloatArray &gpCoords) const;

    int mapToGridPoint(double x, double x0) const { return 1 + gridSize + ( int ) ceil(gridSize * ( x - x0 ) / suprad - 0.5); }
    double mapToGridCoord(double x, double x0) const { return 1. + gridSize + gridSize * ( x - x0 ) / suprad; }
    double dist2FromGridNode(double x, double y, int j, int i) const { return ( ( x - j ) * ( x - j ) + ( y - i ) * ( y - i ) ); }
};
} // end namespace oofem
#endif // nonlocalmaterialext_h
