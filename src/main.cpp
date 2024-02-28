#include <random>
#include <iostream>
#include "WorkerInterface.hpp"
#include <gtest/gtest.h>


Matrix DummyFunction(Matrix matrix) {
    return matrix;
}

bool operator==(const Matrix& lhs, const Matrix& rhs) {
    return lhs.data == rhs.data && lhs.width == rhs.width && lhs.height == rhs.height;
}


TEST(TransposeMatrixTest, TransposeCorrectness) {
        Matrix original;
        original.width = 3;
        original.height = 2;
        original.data = {1, 2, 3, 4, 5, 6};

        Matrix expected;
        expected.width = 2;
        expected.height = 3;
        expected.data = {1, 4, 2, 5, 3, 6};

        Matrix result = TransposeMatrix(original);

        ASSERT_EQ(result, expected);
}
TEST(TransposeMatrixTest, ZeroMatrix) {
    Matrix original;
    original.width = 0;
    original.height = 0;
    original.data = {};

    Matrix expected;
    expected.width = 0;
    expected.height = 0;
    expected.data = {};

    Matrix result = TransposeMatrix(original);

    ASSERT_EQ(result, expected);
}
TEST(TransposeMatrixTest, ColumnMatrix) {
    Matrix original;
    original.width = 1;
    original.height = 10;
    original.data = {1,2,3,4,5,6,7,8,9,10};

    Matrix expected;
    expected.width = 10;
    expected.height = 1;
    expected.data = {1,2,3,4,5,6,7,8,9,10};

    Matrix result = TransposeMatrix(original);

    ASSERT_EQ(result,expected);
}
TEST(TransposeMatrixTest, LineMatrix) {
    Matrix original;
    original.width = 10;
    original.height = 1;
    original.data = {1,2,3,4,5,6,7,8,9,10};

    Matrix expected;
    expected.width = 1;
    expected.height = 10;
    expected.data = {1,2,3,4,5,6,7,8,9,10};

    Matrix result = TransposeMatrix(original);

    ASSERT_EQ(result, expected);
}
TEST(TransposeMatrixTest, Random) {
    for (int i =0; i<1000;i++) {
        srand((unsigned) time(NULL));
        unsigned width = 1 + rand() % 151;
        unsigned height = 1 + rand() % 151;

        std::vector<int> data(width * height);
        for (size_t index = 0; index < width * height; index++)
            data[index] = rand() % 101;
        Matrix original{.data = std::move(data), .width = width, .height = height};
        //Двойное транспонирование дает исхлдный результат.
        Matrix result = TransposeMatrix(TransposeMatrix(original));
        ASSERT_EQ(result, original);
    }
}



TEST(QueueTest, PushTest) {
    threads::safe::Queue<int, 5> queue;
    ASSERT_TRUE(queue.Push(1));
    ASSERT_TRUE(queue.Push(2));
    ASSERT_TRUE(queue.Push(3));
    ASSERT_TRUE(queue.Push(4));
    ASSERT_TRUE(queue.Push(5));
    ASSERT_FALSE(queue.Push(6));
}
TEST(QueueTest, ExtractTest) {
    threads::safe::Queue<int, 5> queue;
    queue.Push(1);
    queue.Push(2);

    auto val = queue.Extract();
    ASSERT_TRUE(val != nullptr);
    ASSERT_EQ(*val, 1);

    val = queue.Extract();
    ASSERT_TRUE(val != nullptr);
    ASSERT_EQ(*val, 2);

    val = queue.Extract();
    ASSERT_TRUE(val == nullptr);
}
TEST(QueueTest, EmptyAndSizeTest) {
    threads::safe::Queue<int, 5> queue;
    ASSERT_TRUE(queue.Empty());
    ASSERT_EQ(queue.Size(), 0);

    queue.Push(1);
    ASSERT_FALSE(queue.Empty());
    ASSERT_EQ(queue.Size(), 1);

    queue.Push(2);
    ASSERT_EQ(queue.Size(), 2);

    queue.Extract();
    ASSERT_EQ(queue.Size(), 1);

    queue.Extract();
    ASSERT_TRUE(queue.Empty());
    ASSERT_EQ(queue.Size(), 0);
}


TEST(ThreadPoolTest, PushTest) {
    threads::ThreadPool threadPool(4);
    Matrix matrix;
    auto futureResult = threadPool.Push(matrix, DummyFunction);
    Matrix result = futureResult.get();
    ASSERT_EQ(matrix, result);
}

TEST(WorkerTransposeTest, AsyncProcessTest) {
    threads::ThreadPool threadPool(4);
    WorkerTranspose worker(threadPool);
    Matrix original;
    original.width = 3;
    original.height = 2;
    original.data = {1, 2, 3, 4, 5, 6};
    auto futureResult = worker.AsyncProcess(original);

    Matrix result = futureResult.get();
    Matrix expected;
    expected.width = 2;
    expected.height = 3;
    expected.data = {1, 4, 2, 5, 3, 6};

    ASSERT_EQ(expected, result);
}
TEST(WorkerTransposeTest, AsyncAlotProcessTest) {
    threads::ThreadPool threadPool(4);
    WorkerTranspose worker(threadPool);
    int n = 1000;
    std::vector<Matrix> origin(n);
    std::vector<Matrix> transpose(n);
    for (int i =0; i<n;i++) {
        srand((unsigned) time(NULL));
        unsigned width = 1 + rand() % 151;
        unsigned height = 1 + rand() % 151;

        std::vector<int> dataOrigin(width * height);
        std::vector<int> dataTranspose(width * height);
        for (size_t i = 0; i < height; i++) {
            for (size_t j = 0; j < width; j++) {
                int randomNum = 1 + rand() % 101;
                dataOrigin[j + i * width] = randomNum;
                dataTranspose[i + j * height] = randomNum;
            }
        }
        origin[i] = {.data = dataOrigin, .width = width, .height = height};
        transpose[i] = {.data = dataTranspose, .width = height, .height = width};
    }
    std::vector<std::future<Matrix>> result_matrices;
    for (int i = 0; i < n; i++) {
        result_matrices.push_back(worker.AsyncProcess(origin[i]));
    }
    for (int i = 0; i < n; i++) {
        auto res = result_matrices[i].get();
        ASSERT_EQ(res, transpose[i]);
    }
}


TEST(WorkerFactoryTest, CreateWorkerTest) {
    auto worker = WorkerFactory::CreateWorker();
    ASSERT_TRUE(worker != nullptr);
}
TEST(WorkerFactoryTest, CreateWorkerTestTranspose) {
    auto worker = WorkerFactory::CreateWorker();
    auto workerTranspose = std::dynamic_pointer_cast<WorkerTranspose>(worker);
    ASSERT_TRUE(workerTranspose != nullptr);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}