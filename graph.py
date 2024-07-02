import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

# Read the CSV file
df = pd.read_csv('search_benchmark.csv')

# Set up the plot style
plt.style.use('ggplot')

# Function to format x-axis labels in exponential notation
def format_func(value, tick_number):
    return f'{value:.0e}'

# Create a separate graph for each distribution
for dist in df['Distribution'].unique():
    dist_df = df[df['Distribution'] == dist]
    
    fig, ax = plt.subplots(figsize=(12, 8))
    
    # Plot binary search upper bound
    ax.plot(dist_df['Size'], dist_df['StdUpperBound'], label='Binary Search (Upper Bound)', color='blue')
    
    # Plot Eytzinger search upper bound
    ax.plot(dist_df['Size'], dist_df['EytzingerUpperBound'], label='Eytzinger Search (Upper Bound)', color='red')
    
    # Set labels and title
    ax.set_xlabel('Number of Elements', fontsize=12)
    ax.set_ylabel('Time (seconds)', fontsize=12)
    ax.set_title(f'Eytzinger vs Binary Search Performance Comparison\nDistribution: {dist}', fontsize=14)
    ax.legend(fontsize=10)
    
    # Set x-axis to log scale and format labels
    ax.set_xscale('log', base=2)
    ax.xaxis.set_major_formatter(plt.FuncFormatter(format_func))
    
    # Set y-axis to log scale
    ax.set_yscale('log', base=2)
    
    # Add grid for better readability
    ax.grid(True, which="both", ls="-", alpha=0.2)
    
    # Ensure all tick labels are visible
    plt.xticks(fontsize=10)
    plt.yticks(fontsize=10)
    
    # Add text box with scale information
    scale_info = (f"X-axis: Log scale (base 2)\n"
                  f"Y-axis: Log scale (base 2)\n"
                  f"Element range: {dist_df['Size'].min():.0e} to {dist_df['Size'].max():.0e}")
    props = dict(boxstyle='round', facecolor='wheat', alpha=0.5)
    ax.text(0.05, 0.95, scale_info, transform=ax.transAxes, fontsize=10,
            verticalalignment='top', bbox=props)

    plt.tight_layout()
    plt.savefig(f'eytzinger_vs_binary_comparison_{dist}.png', dpi=300)
    plt.close()

    print(f"Graph saved as eytzinger_vs_binary_comparison_{dist}.png")

    # Calculate and print performance ratios
    ratios = dist_df['StdUpperBound'] / dist_df['EytzingerUpperBound']
    print(f"\nPerformance Ratios for {dist} Distribution (Binary Search Time / Eytzinger Search Time):")
    print(f"  Min ratio: {ratios.min():.2f}")
    print(f"  Max ratio: {ratios.max():.2f}")
    print(f"  Mean ratio: {ratios.mean():.2f}")