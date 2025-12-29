#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QTreeWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QLabel>
#include "parse_tree.h"

class JsonVisualizer : public QWidget
{
    Q_OBJECT

public:
    JsonVisualizer(QWidget *parent = nullptr);
    ~JsonVisualizer();

private slots:
    void parseAndVisualize();

private:
    QTextEdit *jsonInput;
    QTreeWidget *parseTreeView;
    QPushButton *parseButton;
    QLabel *statusLabel;
    
    void setupUI();
    void populateTreeView(ParseTreePtr node, QTreeWidgetItem *parent = nullptr);
    void clearTreeView();
};
