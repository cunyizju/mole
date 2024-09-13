import re
import csv

def parse_simulation_log(file_path):
    solution_steps = []
    user_times = []
    iterations = []

    # Regular expressions to match the required information
    step_pattern = re.compile(r'Solving\s+\[step number\s+(\d+.\d+),')
    time_pattern = re.compile(r'EngngModel info: user time consumed by solution step \d+: ([\d.]+)s')
    iteration_pattern = re.compile(r'Equilibrium reached.*in (\d+) iterations')

    with open(file_path, 'r') as file:
        content = file.read()

        # Find all step numbers
        step_matches = step_pattern.findall(content)
        for step in step_matches:
            solution_steps.append(int(float(step)))

        # Find all user times
        time_matches = time_pattern.findall(content)
        for time in time_matches:
            user_times.append(float(time))

        # Find all iterations
        iteration_matches = iteration_pattern.findall(content)
        for iteration in iteration_matches:
            iterations.append(int(iteration))

    # Return the extracted data as a dictionary
    return {
        'Solution Steps': solution_steps,
        'User Times': user_times,
        'Iterations': iterations
    }

def export_to_csv(data, output_file):
    # Create a CSV file and write the header and data
    with open(output_file, mode='w', newline='') as file:
        writer = csv.writer(file)
        
        # Write header
        writer.writerow(['solutionStep', 'userTime_s', 'Iterations'])

        # Write data rows
        for step, time, iteration in zip(data['Solution Steps'], data['User Times'], data['Iterations']):
            writer.writerow([step, time, iteration])

# Example usage:
log_file = 'TPB_Xie.log'  # Replace with your file path
output_csv = 'effiencity.csv'  # The output CSV file

# Parse the log file and export the data to a CSV file
results = parse_simulation_log(log_file)
export_to_csv(results, output_csv)

print(f"Data exported successfully to {output_csv}")