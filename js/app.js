// DOM Elements
const sidebar = document.getElementById('sidebar');
const toggleSidebarBtn = document.getElementById('toggle-sidebar');
const pageTitle = document.getElementById('page-title');
const pages = document.querySelectorAll('.page');
const navLinks = document.querySelectorAll('.sidebar-nav a');

// Memory Manager instance
let memoryManager;

// Initialize the application
document.addEventListener('DOMContentLoaded', () => {
    // Initialize memory manager
    memoryManager = new MemoryManager();
    
    // Initialize charts
    initCharts();
    
    // Set up event listeners
    setupEventListeners();
    
    // Load initial data
    loadDashboardData();
    
    // Start periodic updates
    startPeriodicUpdates();
});

// Set up event listeners
function setupEventListeners() {
    // Sidebar toggle
    toggleSidebarBtn.addEventListener('click', () => {
        sidebar.classList.toggle('open');
    });
    
    // Navigation
    navLinks.forEach(link => {
        link.addEventListener('click', (e) => {
            e.preventDefault();
            const target = e.currentTarget.getAttribute('href').substring(1);
            navigateToPage(target);
        });
    });
    
    // Quick action buttons
    document.getElementById('run-gc').addEventListener('click', () => {
        memoryManager.runGarbageCollection();
    });
    
    document.getElementById('optimize-memory').addEventListener('click', () => {
        memoryManager.optimizeMemory();
    });
    
    document.getElementById('defragment').addEventListener('click', () => {
        memoryManager.defragmentMemory();
    });
    
    // Settings form
    document.getElementById('save-settings').addEventListener('click', () => {
        saveSettings();
    });
    
    // Range input displays
    document.getElementById('memory-threshold').addEventListener('input', (e) => {
        document.getElementById('memory-threshold-value').textContent = `${e.target.value}%`;
    });
    
    document.getElementById('cpu-limit').addEventListener('input', (e) => {
        document.getElementById('cpu-limit-value').textContent = `${e.target.value}%`;
    });
}

// Navigate to a specific page
function navigateToPage(pageId) {
    // Update active page
    pages.forEach(page => {
        page.classList.remove('active');
    });
    document.getElementById(`${pageId}-page`).classList.add('active');
    
    // Update active nav link
    navLinks.forEach(link => {
        link.parentElement.classList.remove('active');
    });
    document.querySelector(`.sidebar-nav a[href="#${pageId}"]`).parentElement.classList.add('active');
    
    // Update page title
    pageTitle.textContent = pageId.charAt(0).toUpperCase() + pageId.slice(1).replace('-', ' ');
    
    // Close sidebar on mobile
    if (window.innerWidth <= 768) {
        sidebar.classList.remove('open');
    }
    
    // Load page-specific data
    switch (pageId) {
        case 'dashboard':
            loadDashboardData();
            break;
        case 'gc-brain':
            loadGcBrainData();
            break;
        case 'performance':
            loadPerformanceData();
            break;
        case 'settings':
            loadSettingsData();
            break;
        case 'help':
            loadHelpData();
            break;
    }
}

// Load dashboard data
function loadDashboardData() {
    // Get memory usage
    const memoryUsage = memoryManager.getMemoryUsage();
    updateMemoryUsageDisplay(memoryUsage);
    
    // Get GC stats
    const gcStats = memoryManager.getGcStats();
    updateGcStatsDisplay(gcStats);
    
    // Get CPU impact
    const cpuImpact = memoryManager.getCpuImpact();
    updateCpuImpactDisplay(cpuImpact);
    
    // Get fragmentation
    const fragmentation = memoryManager.getFragmentation();
    updateFragmentationDisplay(fragmentation);
    
    // Get GC activities
    const activities = memoryManager.getRecentActivities();
    updateGcActivitiesDisplay(activities);
    
    // Get memory blocks
    const blocks = memoryManager.getAllBlocks();
    updateMemoryBlocksDisplay(blocks);
    
    // Get GC algorithms
    const algorithms = memoryManager.getAllAlgorithms();
    updateGcAlgorithmsDisplay(algorithms);
    
    // Update memory usage chart
    updateMemoryUsageChart(memoryManager.getMemoryHistory());
}

// Update memory usage display
function updateMemoryUsageDisplay(memoryUsage) {
    const usedPercentage = (memoryUsage.used / memoryUsage.total) * 100;
    
    document.getElementById('memory-used-bar').style.width = `${usedPercentage}%`;
    document.getElementById('memory-used').textContent = formatMemorySize(memoryUsage.used);
    document.getElementById('memory-total').textContent = formatMemorySize(memoryUsage.total);
}

// Update GC stats display
function updateGcStatsDisplay(gcStats) {
    document.getElementById('gc-runs-today').textContent = gcStats.runsToday;
    document.getElementById('gc-last-run').textContent = gcStats.lastRun || 'Never';
    document.getElementById('gc-avg-duration').textContent = `${gcStats.avgDuration} ms`;
}

// Update CPU impact display
function updateCpuImpactDisplay(cpuImpact) {
    document.getElementById('cpu-impact-bar').style.width = `${cpuImpact}%`;
    document.getElementById('cpu-impact-current').textContent = `${cpuImpact}%`;
}

// Update fragmentation display
function updateFragmentationDisplay(fragmentation) {
    document.getElementById('fragmentation-bar').style.width = `${fragmentation.percentage}%`;
    document.getElementById('fragmentation-current').textContent = `${fragmentation.percentage}%`;
    document.getElementById('fragmentation-level').textContent = fragmentation.level;
}

// Update GC activities display
function updateGcActivitiesDisplay(activities) {
    const tbody = document.getElementById('gc-activities-body');
    tbody.innerHTML = '';
    
    activities.forEach(activity => {
        const row = document.createElement('tr');
        row.innerHTML = `
            <td>${formatDate(activity.timestamp)}</td>
            <td>${activity.algorithm}</td>
            <td>${activity.duration} ms</td>
            <td>${formatMemorySize(activity.memoryReclaimed)}</td>
            <td>${activity.objectsCollected}</td>
            <td>${activity.cpuImpact}%</td>
        `;
        tbody.appendChild(row);
    });
}

// Update memory blocks display
function updateMemoryBlocksDisplay(blocks) {
    const container = document.getElementById('memory-blocks');
    container.innerHTML = '';
    
    blocks.forEach(block => {
        const blockElement = document.createElement('div');
        blockElement.className = `memory-block ${block.status}`;
        blockElement.style.width = `${block.size}px`;
        blockElement.style.height = '20px';
        blockElement.style.margin = '2px';
        blockElement.style.display = 'inline-block';
        blockElement.style.backgroundColor = getBlockColor(block.status);
        blockElement.title = `Block ID: ${block.id}, Size: ${formatMemorySize(block.size)}, Status: ${block.status}`;
        container.appendChild(blockElement);
    });
}

// Update GC algorithms display
function updateGcAlgorithmsDisplay(algorithms) {
    const container = document.getElementById('gc-algorithms-list');
    container.innerHTML = '';
    
    algorithms.forEach(algorithm => {
        const algorithmElement = document.createElement('div');
        algorithmElement.className = 'algorithm-item';
        algorithmElement.innerHTML = `
            <div class="algorithm-header">
                <h5>${algorithm.name}</h5>
                <div class="algorithm-status ${algorithm.enabled ? 'enabled' : 'disabled'}">
                    ${algorithm.enabled ? 'Enabled' : 'Disabled'}
                </div>
            </div>
            <p>${algorithm.description}</p>
            <div class="algorithm-performance">
                <span>Performance Score:</span>
                <div class="performance-bar">
                    <div class="performance-level" style="width: ${algorithm.performanceScore}%"></div>
                </div>
                <span>${algorithm.performanceScore}%</span>
            </div>
        `;
        container.appendChild(algorithmElement);
    });
}

// Load settings data
function loadSettingsData() {
    const settings = memoryManager.getSettings();
    updateSettingsForm(settings);
    setupSettingsEventListeners();
}

function updateSettingsForm(settings) {
    // Update GC Settings
    document.getElementById('auto-collection').checked = settings.autoCollection;
    document.getElementById('memory-threshold').value = settings.memoryThreshold;
    document.getElementById('time-interval').value = settings.timeInterval;
    document.getElementById('background-collection').checked = settings.backgroundCollection;
    document.getElementById('cpu-limit').value = settings.cpuLimit;
    document.getElementById('collection-priority').value = settings.collectionPriority;

    // Update Advanced Settings
    document.getElementById('default-algorithm').value = settings.defaultAlgorithm;
    document.getElementById('memory-compaction').checked = settings.memoryCompaction;
    document.getElementById('detailed-logging').checked = settings.detailedLogging;
    document.getElementById('enable-notifications').checked = settings.enableNotifications;
}

function setupSettingsEventListeners() {
    // Save settings button
    document.getElementById('settings-save').addEventListener('click', saveSettings);

    // Add input event listeners for real-time updates
    const inputs = document.querySelectorAll('.settings-form input, .settings-advanced input, .settings-form select, .settings-advanced select');
    inputs.forEach(input => {
        input.addEventListener('change', () => {
            document.getElementById('settings-save').disabled = false;
        });
    });
}

function saveSettings() {
    const settings = {
        autoCollection: document.getElementById('auto-collection').checked,
        memoryThreshold: parseInt(document.getElementById('memory-threshold').value),
        timeInterval: parseInt(document.getElementById('time-interval').value),
        backgroundCollection: document.getElementById('background-collection').checked,
        cpuLimit: parseInt(document.getElementById('cpu-limit').value),
        collectionPriority: document.getElementById('collection-priority').value,
        defaultAlgorithm: document.getElementById('default-algorithm').value,
        memoryCompaction: document.getElementById('memory-compaction').checked,
        detailedLogging: document.getElementById('detailed-logging').checked,
        enableNotifications: document.getElementById('enable-notifications').checked
    };

    memoryManager.updateSettings(settings);
    
    // Show success message
    const saveButton = document.getElementById('settings-save');
    const originalText = saveButton.textContent;
    saveButton.textContent = 'Settings Saved!';
    saveButton.disabled = true;
    
    setTimeout(() => {
        saveButton.textContent = originalText;
    }, 2000);
}

// Start periodic updates
function startPeriodicUpdates() {
    // Update dashboard data every 5 seconds
    setInterval(() => {
        if (document.getElementById('dashboard-page').classList.contains('active')) {
            loadDashboardData();
        }
    }, 5000);
}

// Utility functions
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

function formatDate(dateString) {
    const date = new Date(dateString);
    return date.toLocaleString();
}

function getBlockColor(status) {
    switch (status) {
        case 'active':
            return '#4a6cf7';
        case 'free':
            return '#28a745';
        case 'fragmented':
            return '#ffc107';
        default:
            return '#6c757d';
    }
}

function showNotification(message, type = 'info') {
    // Create notification element
    const notification = document.createElement('div');
    notification.className = `notification ${type} fade-in`;
    notification.textContent = message;
    
    // Add to document
    document.body.appendChild(notification);
    
    // Remove after 3 seconds
    setTimeout(() => {
        notification.classList.remove('fade-in');
        notification.classList.add('fade-out');
        setTimeout(() => {
            document.body.removeChild(notification);
        }, 300);
    }, 3000);
}

// Load GC Brain data
function loadGcBrainData() {
    // Get GC algorithms and their performance
    const algorithms = memoryManager.getAllAlgorithms();
    updateGcAlgorithmsDisplay(algorithms, 'gc-brain-algorithms-list');
    
    // Get memory blocks visualization
    const blocks = memoryManager.getAllBlocks();
    updateMemoryBlocksDisplay(blocks, 'gc-brain-memory-blocks');
    
    // Get GC activities
    const activities = memoryManager.getRecentActivities();
    updateGcActivitiesDisplay(activities, 'gc-brain-activities-body');
    
    // Update GC Brain specific charts
    updateGcPerformanceChart(memoryManager.getGcHistory());
    updateMemoryAllocationChart(memoryManager.getAllocationHistory());
}

// Load Performance data
function loadPerformanceData() {
    // Get CPU impact
    const cpuImpact = memoryManager.getCpuImpact();
    updateCpuImpactDisplay(cpuImpact);
    
    // Get memory usage
    const memoryUsage = memoryManager.getMemoryUsage();
    updateMemoryUsageDisplay(memoryUsage);
    
    // Get fragmentation
    const fragmentation = memoryManager.getFragmentation();
    updateFragmentationDisplay(fragmentation);
    
    // Update performance charts
    updateCpuUsageChart(memoryManager.getCpuHistory());
    updateMemoryTrendChart(memoryManager.getMemoryHistory());
    
    // Get performance metrics
    const metrics = memoryManager.getPerformanceMetrics();
    updatePerformanceMetrics(metrics);
}

// Load Help data
function loadHelpData() {
    // Get help documentation
    const documentation = {
        sections: [
            {
                title: 'Getting Started',
                content: 'Welcome to MemoryMaster! This tool helps you monitor and optimize memory usage in your application.'
            },
            {
                title: 'Dashboard',
                content: 'The dashboard provides real-time monitoring of memory usage, GC activities, and system performance.'
            },
            {
                title: 'GC Brain',
                content: 'The GC Brain section shows detailed information about garbage collection algorithms and memory blocks.'
            },
            {
                title: 'Performance',
                content: 'Monitor CPU impact, memory usage trends, and system performance metrics.'
            },
            {
                title: 'Settings',
                content: 'Configure garbage collection settings, thresholds, and optimization parameters.'
            }
        ]
    };
    
    updateHelpContent(documentation);
}

// Update GC performance chart
function updateGcPerformanceChart(gcHistory) {
    const ctx = document.getElementById('gc-performance-chart').getContext('2d');
    new Chart(ctx, {
        type: 'line',
        data: {
            labels: gcHistory.map(h => formatDate(h.timestamp)),
            datasets: [{
                label: 'GC Duration (ms)',
                data: gcHistory.map(h => h.duration),
                borderColor: '#4a6cf7',
                tension: 0.1
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false
        }
    });
}

// Update memory allocation chart
function updateMemoryAllocationChart(allocationHistory) {
    const ctx = document.getElementById('memory-allocation-chart').getContext('2d');
    new Chart(ctx, {
        type: 'bar',
        data: {
            labels: allocationHistory.map(h => formatDate(h.timestamp)),
            datasets: [{
                label: 'Allocated Memory',
                data: allocationHistory.map(h => h.allocated),
                backgroundColor: '#4a6cf7'
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false
        }
    });
}

// Update CPU usage chart
function updateCpuUsageChart(cpuHistory) {
    const ctx = document.getElementById('cpu-usage-chart').getContext('2d');
    new Chart(ctx, {
        type: 'line',
        data: {
            labels: cpuHistory.map(h => formatDate(h.timestamp)),
            datasets: [{
                label: 'CPU Usage (%)',
                data: cpuHistory.map(h => h.usage),
                borderColor: '#ff6b6b',
                tension: 0.1
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false
        }
    });
}

// Update memory trend chart
function updateMemoryTrendChart(memoryHistory) {
    const ctx = document.getElementById('memory-trend-chart').getContext('2d');
    new Chart(ctx, {
        type: 'line',
        data: {
            labels: memoryHistory.map(h => formatDate(h.timestamp)),
            datasets: [{
                label: 'Memory Usage',
                data: memoryHistory.map(h => h.used),
                borderColor: '#28a745',
                tension: 0.1
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false
        }
    });
}

// Update performance metrics
function updatePerformanceMetrics(metrics) {
    document.getElementById('avg-response-time').textContent = `${metrics.avgResponseTime}ms`;
    document.getElementById('peak-memory-usage').textContent = formatMemorySize(metrics.peakMemoryUsage);
    document.getElementById('gc-efficiency').textContent = `${metrics.gcEfficiency}%`;
    document.getElementById('memory-leaks').textContent = metrics.memoryLeaks ? 'Detected' : 'None';
}

// Update help content
function updateHelpContent(documentation) {
    const container = document.getElementById('help-content');
    container.innerHTML = '';
    
    documentation.sections.forEach(section => {
        const sectionElement = document.createElement('div');
        sectionElement.className = 'help-section';
        sectionElement.innerHTML = `
            <h3>${section.title}</h3>
            <p>${section.content}</p>
        `;
        container.appendChild(sectionElement);
    });
}

// Update GC algorithms display
function updateGcAlgorithmsDisplay(algorithms, containerId = 'gc-algorithms-list') {
    const container = document.getElementById(containerId);
    container.innerHTML = '';
    
    algorithms.forEach(algorithm => {
        const algorithmElement = document.createElement('div');
        algorithmElement.className = 'algorithm-item';
        algorithmElement.innerHTML = `
            <div class="algorithm-header">
                <h5>${algorithm.name}</h5>
                <div class="algorithm-status ${algorithm.enabled ? 'enabled' : 'disabled'}">
                    ${algorithm.enabled ? 'Enabled' : 'Disabled'}
                </div>
            </div>
            <p>${algorithm.description}</p>
            <div class="algorithm-performance">
                <span>Performance Score:</span>
                <div class="performance-bar">
                    <div class="performance-level" style="width: ${algorithm.performanceScore}%"></div>
                </div>
                <span>${algorithm.performanceScore}%</span>
            </div>
        `;
        container.appendChild(algorithmElement);
    });
}

// Update memory blocks display
function updateMemoryBlocksDisplay(blocks, containerId = 'memory-blocks') {
    const container = document.getElementById(containerId);
    container.innerHTML = '';
    
    blocks.forEach(block => {
        const blockElement = document.createElement('div');
        blockElement.className = `memory-block ${block.status}`;
        blockElement.style.width = `${block.size}px`;
        blockElement.style.height = '20px';
        blockElement.style.margin = '2px';
        blockElement.style.display = 'inline-block';
        blockElement.style.backgroundColor = getBlockColor(block.status);
        blockElement.title = `Block ID: ${block.id}, Size: ${formatMemorySize(block.size)}, Status: ${block.status}`;
        container.appendChild(blockElement);
    });
}

// Update GC activities display
function updateGcActivitiesDisplay(activities, containerId = 'gc-activities-body') {
    const tbody = document.getElementById(containerId);
    tbody.innerHTML = '';
    
    activities.forEach(activity => {
        const row = document.createElement('tr');
        row.innerHTML = `
            <td>${formatDate(activity.timestamp)}</td>
            <td>${activity.algorithm}</td>
            <td>${activity.duration} ms</td>
            <td>${formatMemorySize(activity.memoryReclaimed)}</td>
            <td>${activity.objectsCollected}</td>
            <td>${activity.cpuImpact}%</td>
        `;
        tbody.appendChild(row);
    });
} 