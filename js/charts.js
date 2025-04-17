// Chart.js library is required for this file to work
// Make sure to include it in the HTML file

// Memory usage chart
let memoryUsageChart;

// Initialize charts
function initCharts() {
    initMemoryUsageChart();
}

// Initialize memory usage chart
function initMemoryUsageChart() {
    const ctx = document.getElementById('memory-usage-chart').getContext('2d');
    
    memoryUsageChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: [],
            datasets: [
                {
                    label: 'Used Memory',
                    data: [],
                    borderColor: '#4a6cf7',
                    backgroundColor: 'rgba(74, 108, 247, 0.1)',
                    borderWidth: 2,
                    fill: true,
                    tension: 0.4
                },
                {
                    label: 'Free Memory',
                    data: [],
                    borderColor: '#28a745',
                    backgroundColor: 'rgba(40, 167, 69, 0.1)',
                    borderWidth: 2,
                    fill: true,
                    tension: 0.4
                }
            ]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            plugins: {
                legend: {
                    position: 'top',
                },
                tooltip: {
                    mode: 'index',
                    intersect: false,
                    callbacks: {
                        label: function(context) {
                            return `${context.dataset.label}: ${formatMemorySize(context.raw)}`;
                        }
                    }
                }
            },
            scales: {
                x: {
                    display: true,
                    title: {
                        display: true,
                        text: 'Time'
                    }
                },
                y: {
                    display: true,
                    title: {
                        display: true,
                        text: 'Memory'
                    },
                    ticks: {
                        callback: function(value) {
                            return formatMemorySize(value);
                        }
                    }
                }
            }
        }
    });
}

// Update memory usage chart with new data
function updateMemoryUsageChart(memoryHistory) {
    if (!memoryUsageChart) return;
    
    // Extract data from memory history
    const labels = memoryHistory.map(record => formatTime(record.timestamp));
    const usedMemoryData = memoryHistory.map(record => record.used);
    const freeMemoryData = memoryHistory.map(record => record.free);
    
    // Update chart data
    memoryUsageChart.data.labels = labels;
    memoryUsageChart.data.datasets[0].data = usedMemoryData;
    memoryUsageChart.data.datasets[1].data = freeMemoryData;
    
    // Update chart
    memoryUsageChart.update();
}

// Format time for chart labels
function formatTime(timestamp) {
    const date = new Date(timestamp);
    return date.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit', second: '2-digit' });
}

// Format memory size for tooltips
function formatMemorySize(bytes) {
    const units = ['B', 'KB', 'MB', 'GB', 'TB'];
    let size = bytes;
    let unitIndex = 0;
    
    while (size >= 1024 && unitIndex < units.length - 1) {
        size /= 1024;
        unitIndex++;
    }
    
    return `${size.toFixed(2)} ${units[unitIndex]}`;
} 