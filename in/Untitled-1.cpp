#include <bits/stdc++.h>
using namespace std;

// 数据记录结构
struct Record {
    vector<string> features; // 特征值
    string label;            // 类别标签
};

vector<string> attributes; // 条件属性名称
string className;          // 类别属性名称

// 统计训练集中各类别的数量
map<string, int> classCounts(const vector<Record>& rows) {
    map<string, int> counts;
    for (const auto& row : rows) {
        counts[row.label]++;
    }
    return counts;
}

int main() {
    cout << "========== 朴素贝叶斯分类算法 ==========" << endl;
    
    int n, m;
    cout << "请输入训练样本数量：";
    cin >> n;
    cout << "请输入条件属性数量：";
    cin >> m;
    
    attributes.resize(m);
    cout << "请输入 " << m << " 个条件属性名称（空格分隔）：" << endl;
    for (int i = 0; i < m; i++) cin >> attributes[i];
    
    cout << "请输入分类属性名称：";
    cin >> className;
    
    vector<Record> rows(n);
    cout << "请输入每行训练数据：先输入 " << m << " 个条件属性值，最后输入类别值。" << endl;
    for (int i = 0; i < n; i++) {
        rows[i].features.resize(m);
        for (int j = 0; j < m; j++) {
            cin >> rows[i].features[j];
        }
        cin >> rows[i].label;
    }
    
    cout << "\n请输入待分类的未知样本：依次输入 " << m << " 个条件属性值：" << endl;
    vector<string> testSample(m);
    for (int i = 0; i < m; i++) cin >> testSample[i];
    
    cout << fixed << setprecision(3); // 保留3位小数，方便对齐书本结果
    
    // 1. 统计类别先验概率
    map<string, int> counts = classCounts(rows);
    map<string, double> priors;
    
    cout << "\n========== 第一步：计算先验概率 P(C_i) ==========" << endl;
    for (const auto& item : counts) {
        priors[item.first] = (double)item.second / n;
        cout << "P(" << className << "=\"" << item.first << "\") = " 
             << item.second << "/" << n << " = " << priors[item.first] << endl;
    }
    
    // 2. 计算条件概率 P(X_k | C_i) 并计算最终后验概率分子
    map<string, double> posteriorNumerators;
    
    cout << "\n========== 第二步：计算各属性条件概率 P(X_k|C_i) ==========" << endl;
    for (const auto& classItem : counts) {
        string currentClass = classItem.first;
        int classTotal = classItem.second;
        
        cout << "\n对于类别 " << className << " = \"" << currentClass << "\"：" << endl;
        
        double pxGivenC = 1.0;
        string formulaStr = "";
        
        for (int i = 0; i < m; i++) {
            int featureCount = 0;
            // 统计该类别下，该特征等于测试样本特征值的数量
            for (const auto& row : rows) {
                if (row.label == currentClass && row.features[i] == testSample[i]) {
                    featureCount++;
                }
            }
            double prob = (double)featureCount / classTotal;
            pxGivenC *= prob;
            
            cout << "  P(" << attributes[i] << "=\"" << testSample[i] << "\" | " 
                 << className << "=\"" << currentClass << "\") = " 
                 << featureCount << "/" << classTotal << " = " << prob << endl;
                 
            if (!formulaStr.empty()) formulaStr += " * ";
            formulaStr += to_string(prob).substr(0, 5); // 截断一下只显示几位方便排版
        }
        
        cout << "  -> P(X | " << className << "=\"" << currentClass << "\") = " 
             << formulaStr << " = " << pxGivenC << endl;
             
        double numerator = pxGivenC * priors[currentClass];
        posteriorNumerators[currentClass] = numerator;
        
        cout << "  -> P(X | " << className << "=\"" << currentClass << "\") * P(" 
             << className << "=\"" << currentClass << "\") = " 
             << pxGivenC << " * " << priors[currentClass] << " = " << numerator << endl;
    }
    
    // 3. 比较得出最终分类结果
    cout << "\n========== 第三步：分类预测结果 ==========" << endl;
    string bestClass;
    double maxProb = -1.0;
    
    for (const auto& item : posteriorNumerators) {
        if (item.second > maxProb) {
            maxProb = item.second;
            bestClass = item.first;
        }
    }
    
    cout << "由于 ";
    for (const auto& item : posteriorNumerators) {
        cout << "P(X|" << item.first << ")P(" << item.first << ") = " << item.second << " ";
        if (item.first != bestClass) cout << "< ";
        else cout << "最大 ";
    }
    cout << endl;
    
    cout << "因此，对于待测样本 X，朴素贝叶斯分类预测类别为：" << className << " = \"" << bestClass << "\"" << endl;
    cout << "\n算法执行结束。" << endl;
    
    return 0;
}