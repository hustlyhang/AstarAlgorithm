#pragma once
#include <vector>
#include <list>
#include <queue>
#include <unordered_set>
const int diagonalCost = 14;
const int axialCost = 10;

struct SPairHash {
	template <class T1, class T2>
	size_t operator ()(std::pair<T1, T2> const& pair) const {
		size_t h1 = std::hash<T1>()(pair.first);
		size_t h2 = std::hash<T2>()(pair.second);
		return h1 ^ h2;
	}
};

class CGraph {
public:
	CGraph(std::vector<std::vector<int>>& _data) :m_vData(_data) {
		if (m_vData.size() == 0 || m_vData[0].size() == 0) {
			perror("the graph is wrong\n");
			exit(1);
		}
		m_iCols = m_vData[0].size();
		m_iRows = m_vData.size();
	};

	// 获取图中当前点周围的点
	std::vector<std::pair<int, int>> GetNeighbors(std::pair<int, int> _pos) {
		int posX = _pos.first;
		int posY = _pos.second;
		std::vector<std::pair<int, int>> ret;
		std::vector<std::pair<int, int>> tmp{ {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
		auto fun = [=](int x, int y) {return x < m_iRows&& x >= 0 && y < m_iCols&& y >= 0; };
		for (auto p : tmp) {
			int tmpX = posX + p.first;
			int tmpY = posY + p.second;
			if (fun(tmpX, tmpY) && !m_vData[tmpX][tmpY])
				ret.push_back({ tmpX , tmpY });
		}
		tmp = { {1, 1}, {-1, 1}, {1, -1}, {-1, -1} };
		for (auto p : tmp) {
			int tmpX = posX + p.first;
			int tmpY = posY + p.second;
			if (fun(tmpX, tmpY) && !m_vData[tmpX][tmpY] && !m_vData[posX][posY + p.second] && !m_vData[posX + p.first][posY])
				ret.push_back({ tmpX , tmpY });
		}
		return ret;
	}
private:
	std::vector<std::vector<int>> m_vData;
	int m_iCols;
	int m_iRows;
};

struct Node {
	int x, y;
	int F, H, G;
	Node* parent;// 为了方便重现路径
	Node(int _x, int _y):x(_x), y(_y), parent(nullptr) {};
	bool operator < (const Node& b) {
		return this->F > b.F;
	}
};

class CAStar {
public:
	CAStar(CGraph* _graph, std::pair<int, int> _startPos, std::pair<int, int> _endPos):
		m_cGraph(_graph), m_pStartPos(_startPos), m_pEndPos(_endPos) {};

	// 距离因子，暂且使用曼哈顿距离
	int GetH(Node* _pos) {
		return std::abs(_pos->x - m_pEndPos.first) + std::abs(_pos->y - m_pEndPos.second);
	}
	int GetG(Node* _tmp, Node* _tge) {
		int addG = (std::abs(_tmp->x - _tge->x) + std::abs(_tmp->y - _tge->y)) == 1 ? axialCost : diagonalCost;
		int parentG = _tge->parent == nullptr ? 0 : _tge->parent->G;
		return addG + parentG;
	}

	int GetF(Node* _node) {
		return _node->G + _node->H;
	}

	Node* FindPath() {
		// 先将起始点加到堆中
		Node* fstnode = new Node(m_pStartPos.first, m_pStartPos.second);
		m_qOpenPos.push(fstnode);
		m_sOpenPos.insert({ fstnode->x, fstnode->y });
		while (!m_qOpenPos.empty()) {
			auto node = m_qOpenPos.top();
			m_qOpenPos.pop();
			m_sClosePos.insert({ node->x, node->y });
			// 获取当前节点的周围节点
			auto nodeNeib = m_cGraph->GetNeighbors({ node->x, node->y });
			for (auto n : nodeNeib) {
				if (m_sClosePos.find(n) != m_sClosePos.end()) continue;
				// 如果不在openPos里面，就添加到其中
				Node* newNode = new Node(n.first, n.second);
				if (m_sOpenPos.find(n) == m_sOpenPos.end()) {
					newNode->parent = node;
					newNode->H = GetH(newNode);
					newNode->H = GetG(node, newNode);
					newNode->H = GetF(newNode);
					m_sOpenPos.insert({ newNode->x, newNode->y });
					m_qOpenPos.push(newNode);
					if (n == m_pEndPos) {
						return newNode;
					}
				}
				else {
					// 更新
					int tG = GetG(node, newNode);
					if (tG < newNode->G) {
						newNode->parent = node;
						newNode->G = tG;
						newNode->F = GetF(newNode);
					}
				}
			}
		}
	}
	std::vector<std::pair<int, int>> GetPath() {
		Node* endNode = FindPath();
		std::vector<std::pair<int, int>> path;
		while (endNode) {
			path.push_back({ endNode->x, endNode->y });
			endNode = endNode->parent;
		}
		return path;
	}
private:
	std::pair<int, int> m_pStartPos;
	std::pair<int, int> m_pEndPos;
	CGraph* m_cGraph;
	// 未访问过的点
	std::priority_queue<Node*> m_qOpenPos;
	// 访问过或者是障碍的位置
	std::unordered_set<std::pair<int, int>, SPairHash> m_sClosePos;
	std::unordered_set<std::pair<int, int>, SPairHash> m_sOpenPos;
};
