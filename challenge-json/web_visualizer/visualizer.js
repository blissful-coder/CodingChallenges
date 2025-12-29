// Main visualizer logic
class TreeVisualizer {
    constructor() {
        this.treeView = document.getElementById('treeView');
        this.jsonInput = document.getElementById('jsonInput');
        this.parseButton = document.getElementById('parseButton');
        this.status = document.getElementById('status');
        
        this.setupEventListeners();
    }

    setupEventListeners() {
        this.parseButton.addEventListener('click', () => this.parseAndVisualize());
        
        // Allow parsing with Ctrl+Enter
        this.jsonInput.addEventListener('keydown', (e) => {
            if (e.ctrlKey && e.key === 'Enter') {
                this.parseAndVisualize();
            }
        });
    }

    parseAndVisualize() {
        const jsonText = this.jsonInput.value.trim();
        
        if (!jsonText) {
            this.showStatus('Please enter some JSON to parse', 'warning');
            this.clearTree();
            return;
        }

        try {
            this.parseButton.disabled = true;
            this.parseButton.textContent = 'Parsing...';
            
            // Small delay to show the parsing state
            setTimeout(() => {
                try {
                    const parser = new JsonParser(jsonText);
                    const parseTree = parser.parse();
                    
                    this.displayTree(parseTree);
                    this.showStatus('JSON parsed successfully!', 'success');
                } catch (error) {
                    this.showStatus(`Parse error: ${error.message}`, 'error');
                    this.clearTree();
                } finally {
                    this.parseButton.disabled = false;
                    this.parseButton.textContent = 'Parse JSON';
                }
            }, 100);
            
        } catch (error) {
            this.showStatus(`Error: ${error.message}`, 'error');
            this.clearTree();
            this.parseButton.disabled = false;
            this.parseButton.textContent = 'Parse JSON';
        }
    }

    showStatus(message, type) {
        this.status.textContent = message;
        this.status.className = `status ${type}`;
    }

    clearTree() {
        this.treeView.innerHTML = '<p style="color: #7f8c8d; text-align: center; margin-top: 2rem;">Parse tree will appear here after clicking "Parse JSON"</p>';
    }

    displayTree(rootNode) {
        this.treeView.innerHTML = '';
        this.renderNode(rootNode, this.treeView, 0, 'root');
    }

    renderNode(node, container, depth, displayName) {
        const nodeDiv = document.createElement('div');
        nodeDiv.className = 'tree-node';
        nodeDiv.style.marginLeft = `${depth * 20}px`;

        // Create the node content
        const nodeContent = document.createElement('div');
        nodeContent.style.display = 'flex';
        nodeContent.style.alignItems = 'center';

        // Add expand/collapse icon for objects and arrays
        const iconSpan = document.createElement('span');
        iconSpan.className = 'node-icon';
        
        let isExpandable = false;
        if (node.type === NodeType.OBJECT && node.children.size > 0) {
            iconSpan.textContent = '▼';
            isExpandable = true;
        } else if (node.type === NodeType.ARRAY && node.arrayItems.length > 0) {
            iconSpan.textContent = '▼';
            isExpandable = true;
        } else {
            iconSpan.textContent = '•';
        }

        // Add node key/name
        const keySpan = document.createElement('span');
        keySpan.className = 'node-key';
        keySpan.textContent = displayName + ': ';

        // Add node type
        const typeSpan = document.createElement('span');
        typeSpan.className = 'node-type';
        typeSpan.textContent = `[${node.getNodeTypeString()}] `;

        // Add node value
        const valueSpan = document.createElement('span');
        valueSpan.className = `node-value ${node.type.toLowerCase()}`;
        valueSpan.textContent = node.getDisplayValue();

        nodeContent.appendChild(iconSpan);
        nodeContent.appendChild(keySpan);
        nodeContent.appendChild(typeSpan);
        nodeContent.appendChild(valueSpan);

        nodeDiv.appendChild(nodeContent);

        // Create children container
        const childrenDiv = document.createElement('div');
        childrenDiv.className = 'node-children';
        
        // Add expand/collapse functionality
        if (isExpandable) {
            nodeDiv.classList.add('expandable');
            let isExpanded = true;
            
            nodeContent.addEventListener('click', () => {
                isExpanded = !isExpanded;
                iconSpan.textContent = isExpanded ? '▼' : '▶';
                childrenDiv.style.display = isExpanded ? 'block' : 'none';
            });

            // Render children
            if (node.type === NodeType.OBJECT) {
                const entries = Array.from(node.children.entries()).sort((a, b) => a[0].localeCompare(b[0]));
                entries.forEach(([key, childNode]) => {
                    this.renderNode(childNode, childrenDiv, depth + 1, key);
                });
            } else if (node.type === NodeType.ARRAY) {
                node.arrayItems.forEach((childNode, index) => {
                    this.renderNode(childNode, childrenDiv, depth + 1, `[${index}]`);
                });
            }
        }

        nodeDiv.appendChild(childrenDiv);
        container.appendChild(nodeDiv);
    }
}

// Example JSON data
const examples = {
    simple: `{
  "name": "John Doe",
  "age": 30,
  "active": true
}`,
    nested: `{
  "user": {
    "name": "Alice Smith",
    "profile": {
      "email": "alice@example.com",
      "preferences": {
        "theme": "dark",
        "notifications": true
      }
    }
  }
}`,
    array: `{
  "fruits": ["apple", "banana", "orange"],
  "numbers": [1, 2, 3, 4, 5],
  "mixed": ["hello", 42, true, null]
}`,
    complex: `{
  "company": "Tech Corp",
  "employees": [
    {
      "id": 1,
      "name": "John Doe",
      "position": "Developer",
      "skills": ["JavaScript", "Python", "C++"],
      "active": true,
      "salary": null
    },
    {
      "id": 2,
      "name": "Jane Smith",
      "position": "Designer",
      "skills": ["Photoshop", "Illustrator"],
      "active": false,
      "salary": 75000
    }
  ],
  "locations": {
    "headquarters": {
      "city": "San Francisco",
      "country": "USA"
    },
    "branch": {
      "city": "London",
      "country": "UK"
    }
  },
  "founded": 2010,
  "public": true
}`
};

// Load example function
function loadExample(type) {
    const jsonInput = document.getElementById('jsonInput');
    if (examples[type]) {
        jsonInput.value = examples[type];
        // Auto-parse after loading example
        setTimeout(() => {
            visualizer.parseAndVisualize();
        }, 100);
    }
}

// Initialize the visualizer when the page loads
let visualizer;
document.addEventListener('DOMContentLoaded', () => {
    visualizer = new TreeVisualizer();
    
    // Load the simple example by default
    setTimeout(() => {
        loadExample('simple');
    }, 500);
});
