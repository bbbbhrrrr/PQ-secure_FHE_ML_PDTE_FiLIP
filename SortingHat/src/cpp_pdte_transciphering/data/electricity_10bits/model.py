import json
import numpy as np
from sklearn.datasets import load_breast_cancer
from sklearn.tree import DecisionTreeClassifier, export_text
from sklearn.model_selection import train_test_split
from sklearn.tree import _tree
from pandas import read_csv

# 加载数据集
X = read_csv("/home/bbbbhrrrr/sortinghat/SortingHat/src/cpp_pdte_transciphering/data/electricity_10bits/x_test.csv")
y = read_csv("/home/bbbbhrrrr/sortinghat/SortingHat/src/cpp_pdte_transciphering/data/electricity_10bits/y_test.csv")

# 划分训练集和测试集
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# 训练决策树模型
clf = DecisionTreeClassifier(random_state=42)
clf.fit(X_train, y_train)



# 将决策树模型转换为所需的 JSON 结构
def tree_to_custom_json(tree):
    tree_ = tree.tree_
    
    def recurse(node):
        if tree_.feature[node] != _tree.TREE_UNDEFINED:
            threshold = tree_.threshold[node]
            return {
                "internal": {
                    "threshold": threshold,
                    "feature": int(tree_.feature[node]),  # 转换为 int
                    "index": int(node),  # 转换为 int
                    "op": "leq",
                    "left": recurse(tree_.children_left[node]),
                    "right": recurse(tree_.children_right[node])
                }
            }
        else:
            return {
                "leaf": int(np.argmax(tree_.value[node]))
            }
    
    return recurse(0)

# 转换为 JSON
tree_json = tree_to_custom_json(clf)

# 保存为 JSON 文件，保存在当前目录下
with open("decision_tree.json", "w") as f:
    json.dump(tree_json, f)

# 读取 JSON 文件，c语言，从json文件中读取决策树



print("决策树模型已保存为 decision_tree.json")