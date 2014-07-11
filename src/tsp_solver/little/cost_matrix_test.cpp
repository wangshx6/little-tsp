#include "tsp_solver/little/cost_matrix.hpp"

#include <cassert>

#include <algorithm>
#include <iterator>
#include <limits>
#include <memory>
#include <vector>

#include "graph/edge_cost.hpp"
#include "edge.hpp"
#include "graph/mock.hpp"
#include "util.hpp"
#include "matrix.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using std::back_inserter;
using std::max_element;
using std::min_element;
using std::numeric_limits;
using std::transform;
using std::unique_ptr;
using std::vector;

/*
template <typename T>
using CostVector = CostMatrix::CostVector<T>;
using Row = CostMatrix::Row;
using Column = CostMatrix::Column;
*/

using ::testing::Const;
using ::testing::Return;
using ::testing::ReturnRef;

const int infinity{numeric_limits<int>::max()};

const vector<Edge> include1;
const vector<Edge> exclude1_edges{{0, 0}};
const vector<Edge> include2{{0, 2}};
const vector<Edge> exclude2_edges{{0, 0}};


const Matrix<EdgeCost> graph_weights{
	MakeEdgeCosts({6, 2, 4, 4, 5, 3, 8, 9, 7}, 3)};

//template<typename T>
//static void CompareWithVector(T matrix, vector<int> expected);

class CostMatrixTest : public ::testing::Test {
public:
	CostMatrixTest();
protected:
	// I need unique_ptrs because graph functions get called in the constructor
	// of matrix, and I need time to set expectations for mock calls
	unique_ptr<CostMatrix> matrix1_ptr, matrix2_ptr;

private:
	MockGraph graph;
	Matrix<int> exclude1, exclude2;
};

CostMatrixTest::CostMatrixTest() {
	// set expectations for the graph
	for (int i{0}; i < graph_weights.GetNumRows(); ++i) {
		for (int j{0}; j < graph_weights.GetNumColumns(); ++j) {
			EXPECT_CALL(Const(graph), Predicate(i, j)).WillRepeatedly(
					ReturnRef(graph_weights(i, j)));
		}
	}

	EXPECT_CALL(graph, GetNumVertices()).WillRepeatedly(Return(3));

	exclude1.SetSize(3);
	exclude2.SetSize(3);
	for (const Edge& e : exclude1_edges) { exclude1(e.u, e.v) = 1; }
	for (const Edge& e : exclude2_edges) { exclude2(e.u, e.v) = 1; }

	// construct the matrix after we set expectations for the graph
	matrix1_ptr = unique_ptr<CostMatrix>{
		new CostMatrix{graph, include1, exclude1}};
	matrix2_ptr = unique_ptr<CostMatrix>{
		new CostMatrix{graph, include2, exclude2}};

	// reduce the matrices
	EXPECT_EQ(13, matrix1_ptr->ReduceMatrix());
	EXPECT_EQ(13, matrix2_ptr->ReduceMatrix());
}

TEST_F(CostMatrixTest, Size) {
	EXPECT_EQ(3, matrix1_ptr->Size());
	EXPECT_EQ(2, matrix2_ptr->Size());
}

/*
TEST_F(CostMatrixTest, ReduceMatrix) {
	// make sure the matrices were reduced correctly
	CompareWithVector(*matrix1_ptr, {infinity, 0, 2, 0, 2, 0, 0, 2, 0});
	CompareWithVector(*matrix2_ptr, {0, 0, 0, 0});
}

TEST_F(CostMatrixTest, GetRow) {
	// iterate over matrix1 with rows
	EXPECT_TRUE(matrix1_ptr->IsRowAvailable(0));
	EXPECT_TRUE(matrix1_ptr->IsRowAvailable(1));
	EXPECT_TRUE(matrix1_ptr->IsRowAvailable(2));

	CostMatrix::CostVector<Row> row0{matrix1_ptr->GetRow(0)};
	CostMatrix::CostVector<Row> row1{matrix1_ptr->GetRow(1)};
	CostMatrix::CostVector<Row> row2{matrix1_ptr->GetRow(2)};
	CompareWithVector(row0, {infinity, 0, 2});
	CompareWithVector(row1, {0, 2, 0});
	CompareWithVector(row2, {0, 2, 0});

	auto row2_max_it = max_element(row2.begin(), row2.end());
	int row2_max{row2_max_it->operator()()};
	EXPECT_EQ(2, row2_max);
}

TEST_F(CostMatrixTest, GetColumn) {
	// iterate over matrix2 with columns
	EXPECT_TRUE(matrix2_ptr->IsColumnAvailable(0));
	EXPECT_TRUE(matrix2_ptr->IsColumnAvailable(1));
	EXPECT_FALSE(matrix2_ptr->IsColumnAvailable(2));
	CostMatrix::CostVector<Column> column0{matrix2_ptr->GetColumn(0)};
	CostMatrix::CostVector<Column> column1{matrix2_ptr->GetColumn(1)};
	EXPECT_THROW(matrix2_ptr->GetColumn(2), NotAvailableError);
	CompareWithVector(column0, {0, 0});
	CompareWithVector(column1, {0, 0});

	auto column1_max_it = max_element(column1.begin(), column1.end());
	int column1_max{column1_max_it->operator()()};
	EXPECT_EQ(0, column1_max);
}

template<typename T>
void TestMatrix(const CostMatrix& matrix, vector<int> expected) {
	vector<int> actual;
	transform(matrix.begin(), matrix.end(), back_inserter(actual),
			[](const EdgeCost& cmi) { return cmi(); });
	EXPECT_EQ(expected, actual);
}
*/
