import subprocess
import re
import matplotlib.pyplot as plt
import math

results = []

# Sampling every 100th value in the range from 12 to 10 million
from_num = 10
to_num = 100_000
step_num = 2 * math.floor(math.sqrt(to_num - from_num))
for i in range(from_num, to_num, step_num):
    command = f"time build/out benchmark {i}"
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    process.wait()
    output = process.stderr.read().decode().strip()

    time_info = re.findall(r"(\d+\.\d+)(m\ds)?", output)
    if time_info:
        time_value = float(time_info[-1][0])
        results.append((i, time_value))

x_values, y_values = zip(*results)

plt.figure(figsize=(8, 6))
plt.scatter(x_values, y_values)
plt.xlabel('Parameter Value')
plt.ylabel('Execution Time (s)')
plt.title('Benchmark Results (Sampled)')
plt.grid(True)
plt.show()
