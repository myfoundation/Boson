#pragma once

namespace Boson {

	class BalancedTreeTest {
	public:
		bool run(bool verbose = true);
	private:
		
		bool assert(char* msg, bool expr);
		
		bool testLeafNode(bool verbose = true);
		bool testInnerNode(bool verbose = true);
		bool testBalancedTree(bool verbose = true);
		bool testPerformance(bool verbose = true);

		BalancedTreeIndex* buildTree(bool verbose);
		bool deleteTree(BalancedTreeIndex* bt, bool verbose);
	};

};

