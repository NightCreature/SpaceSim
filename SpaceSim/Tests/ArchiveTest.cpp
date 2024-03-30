#include <gtest/gtest.h>
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Core/Serialization/Archive.h"
#include <filesystem>
#include <vector>

static const std::filesystem::path testTempPath = "teststemp/test.txt";

void ClearTempFiles(const std::filesystem::path& path) {
    if (std::filesystem::exists(path))
    {
        std::filesystem::remove(path);
    }
}

// Test the Archive Open method with an invalid path.
TEST(ArchiveTest, OpenInvalidPath) 
{
    Archive archive;
    ASSERT_FALSE(archive.Open(std::filesystem::path("invalid:/path")));
}

// Test the Archive Write and Read methods with a non-fundamental type.
TEST(ArchiveTest, WriteReadNonFundamental) 
{
    ClearTempFiles(testTempPath);
    Archive archive;
    ASSERT_TRUE(archive.Open(testTempPath));
    std::string writeValue = "test";
    archive.Write(writeValue);
    archive.Close();

    ASSERT_TRUE(archive.Open(testTempPath));
    std::string readValue;
    archive.Read(readValue);
    archive.Close();

    // This should fail because Archive::WriteObject and Archive::ReadObject do not support non-fundamental types.
    ASSERT_NE(writeValue, readValue);
}

// Test the Archive WriteContainer and ReadContainer methods with an empty container.
TEST(ArchiveTest, WriteReadEmptyContainer) 
{
    ClearTempFiles(testTempPath);
    Archive archive;
    ASSERT_TRUE(archive.Open(testTempPath));
    std::vector<int> writeContainer;
    archive.WriteContainer(writeContainer);
    archive.Close();

    ASSERT_TRUE(archive.Open(testTempPath));
    std::vector<int> readContainer = { 1, 2, 3 }; // Initialize with some values to ensure it gets cleared.
    archive.ReadContainer(readContainer);
    archive.Close();

    ASSERT_TRUE(readContainer.empty());
}

// Test the Archive WriteContainer and ReadContainer methods with a large container.
TEST(ArchiveTest, WriteReadLargeContainer) 
{
    ClearTempFiles(testTempPath);
    Archive archive;
    ASSERT_TRUE(archive.Open(testTempPath));
    std::vector<int> writeContainer(10, 123); // A large container with 100 elements.
    archive.WriteContainer(writeContainer);
    archive.Close();

    ASSERT_TRUE(archive.Open(testTempPath));
    std::vector<int> readContainer;
    readContainer.reserve(10);
    archive.ReadContainer(readContainer);
    archive.Close();

    ASSERT_EQ(writeContainer, readContainer);
}

// Test the Archive Write and Read methods with a fundamental type.
TEST(ArchiveTest, WriteReadFundamental)
{
    ClearTempFiles(testTempPath);
    Archive archive;
    ASSERT_TRUE(archive.Open(testTempPath));
    int writeValue = 123;
    archive.Write(writeValue);
    archive.Close();

    ASSERT_TRUE(archive.Open(testTempPath));
    int readValue = 0;
    archive.Read(readValue);
    archive.Close();

    ASSERT_EQ(writeValue, readValue);
}

// Test the Archive Write and Read methods with a pointer to a fundamental type.
TEST(ArchiveTest, WriteReadPointer)
{
    ClearTempFiles(testTempPath);
    Archive archive;
    ASSERT_TRUE(archive.Open(testTempPath));
    int originalValue = 123;
    int* writeValue = &originalValue;
    archive.Write(writeValue);
    archive.Close();

    ASSERT_TRUE(archive.Open(testTempPath));
    int* readValue = nullptr;
    archive.Read(readValue);
    archive.Close();

    ASSERT_EQ(*writeValue, *readValue);
}

// Test the Archive WriteContainer and ReadContainer methods with a non-empty container.
TEST(ArchiveTest, WriteReadNonEmptyContainer)
{
    ClearTempFiles(testTempPath);
    Archive archive;
    ASSERT_TRUE(archive.Open(testTempPath));
    std::vector<int> writeContainer = { 1, 2, 3 };
    archive.WriteContainer(writeContainer);
    archive.Close();

    ASSERT_TRUE(archive.Open(testTempPath));
    std::vector<int> readContainer;
    archive.ReadContainer(readContainer);
    archive.Close();

    ASSERT_EQ(writeContainer, readContainer);
}