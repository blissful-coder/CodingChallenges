#include "json_visualizer.h"
#include "tree_parser.h"
#include "lexer.h"
#include <QApplication>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QMessageBox>
#include <iostream>

JsonVisualizer::JsonVisualizer(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

JsonVisualizer::~JsonVisualizer()
{
}

void JsonVisualizer::setupUI()
{
    setWindowTitle("JSON Parse Tree Visualizer");
    setMinimumSize(1000, 600);

    // Create main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    
    // Create splitter to divide left and right panels
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    
    // Left panel - JSON input
    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    
    QLabel *inputLabel = new QLabel("JSON Input:");
    jsonInput = new QTextEdit();
    jsonInput->setPlaceholderText("Enter your JSON here...\n\nExample:\n{\n  \"name\": \"John\",\n  \"age\": 30,\n  \"active\": true\n}");
    
    parseButton = new QPushButton("Parse JSON");
    statusLabel = new QLabel("Ready to parse JSON");
    statusLabel->setStyleSheet("color: green; font-weight: bold;");
    
    leftLayout->addWidget(inputLabel);
    leftLayout->addWidget(jsonInput, 1);
    leftLayout->addWidget(parseButton);
    leftLayout->addWidget(statusLabel);
    
    // Right panel - Parse tree view
    QWidget *rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    
    QLabel *treeLabel = new QLabel("Parse Tree:");
    parseTreeView = new QTreeWidget();
    parseTreeView->setHeaderLabels({"Node", "Type", "Value"});
    parseTreeView->header()->setStretchLastSection(false);
    parseTreeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    parseTreeView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    parseTreeView->header()->setSectionResizeMode(2, QHeaderView::Stretch);
    
    rightLayout->addWidget(treeLabel);
    rightLayout->addWidget(parseTreeView, 1);
    
    // Add panels to splitter
    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);
    splitter->setSizes({400, 600});
    
    // Add splitter to main layout
    mainLayout->addWidget(splitter);
    
    // Connect signals
    connect(parseButton, &QPushButton::clicked, this, &JsonVisualizer::parseAndVisualize);
}

void JsonVisualizer::parseAndVisualize()
{
    clearTreeView();
    
    QString jsonText = jsonInput->toPlainText().trimmed();
    if (jsonText.isEmpty()) {
        statusLabel->setText("Please enter some JSON to parse");
        statusLabel->setStyleSheet("color: orange; font-weight: bold;");
        return;
    }
    
    try {
        // Disable lexer output for GUI
        std::cout.setstate(std::ios_base::badbit);
        
        std::string jsonString = jsonText.toStdString();
        Lexer lexer(jsonString);
        TreeParser parser(std::move(lexer));
        
        ParseTreePtr root = parser.parse();
        
        // Re-enable cout
        std::cout.clear();
        
        if (root) {
            populateTreeView(root);
            statusLabel->setText("JSON parsed successfully!");
            statusLabel->setStyleSheet("color: green; font-weight: bold;");
            parseTreeView->expandAll();
        } else {
            statusLabel->setText("Failed to parse JSON - Invalid syntax");
            statusLabel->setStyleSheet("color: red; font-weight: bold;");
        }
    } catch (const std::exception& e) {
        std::cout.clear();
        statusLabel->setText(QString("Parse error: %1").arg(e.what()));
        statusLabel->setStyleSheet("color: red; font-weight: bold;");
    }
}

void JsonVisualizer::populateTreeView(ParseTreePtr node, QTreeWidgetItem *parent)
{
    if (!node) return;
    
    QTreeWidgetItem *item;
    if (parent) {
        item = new QTreeWidgetItem(parent);
    } else {
        item = new QTreeWidgetItem(parseTreeView);
    }
    
    // Set the node display name
    QString nodeName = node->key.empty() ? "Root" : QString::fromStdString(node->key);
    if (node->type == NodeType::ARRAY && !node->key.empty()) {
        nodeName = QString::fromStdString(node->key);
    } else if (node->type == NodeType::ARRAY && node->key.empty() && parent) {
        // For array items, show index
        nodeName = QString("[%1]").arg(parent->childCount() - 1);
    }
    
    item->setText(0, nodeName);
    item->setText(1, QString::fromStdString(node->getNodeTypeString()));
    item->setText(2, QString::fromStdString(node->getDisplayValue()));
    
    // Add children for objects
    if (node->type == NodeType::OBJECT) {
        for (const auto& pair : node->children) {
            populateTreeView(pair.second, item);
        }
    }
    
    // Add items for arrays
    if (node->type == NodeType::ARRAY) {
        for (size_t i = 0; i < node->arrayItems.size(); ++i) {
            auto child = node->arrayItems[i];
            QTreeWidgetItem *arrayItem = new QTreeWidgetItem(item);
            
            arrayItem->setText(0, QString("[%1]").arg(i));
            arrayItem->setText(1, QString::fromStdString(child->getNodeTypeString()));
            arrayItem->setText(2, QString::fromStdString(child->getDisplayValue()));
            
            // If the array item is an object or array, recursively add its children
            if (child->type == NodeType::OBJECT) {
                for (const auto& pair : child->children) {
                    populateTreeView(pair.second, arrayItem);
                }
            } else if (child->type == NodeType::ARRAY) {
                populateTreeView(child, arrayItem);
            }
        }
    }
}

void JsonVisualizer::clearTreeView()
{
    parseTreeView->clear();
}
