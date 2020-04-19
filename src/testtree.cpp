#include <gtest/gtest.h>
#include "FileSystemTree.h"

#include <iostream>

#include "Path.h"
TEST(Path, AlexTests){
	CPath crying(""), slash("/");
	std::cout << std::string(crying) << std::endl;
	std::cout << std::string(slash) << std::endl;
	
}

TEST(FileSystemTree, EmptyTreeTest){
    CFileSystemTree Tree;

    EXPECT_EQ(std::string(Tree), "/");
    EXPECT_EQ(Tree.Root().FullPath(), "/");
    EXPECT_TRUE(Tree.Root().Valid());
    EXPECT_FALSE(Tree.Root().Parent().Valid());
}

TEST(FileSystemTree, AddChildTest){
    CFileSystemTree Tree;

    EXPECT_TRUE(Tree.Root().AddChild("bin"));
    EXPECT_TRUE(Tree.Root().AddChild("etc"));
    EXPECT_TRUE(Tree.Root().AddChild("home"));
    EXPECT_TRUE(Tree.Root().AddChild("dev"));
    EXPECT_TRUE(Tree.Root().AddChild("proc"));
    EXPECT_TRUE(Tree.Root().AddChild("usr"));
    EXPECT_TRUE(Tree.Root().AddChild("var"));
    EXPECT_FALSE(Tree.Root().AddChild("etc"));
    EXPECT_TRUE(Tree.Root().AddChild("/home/cjnitta/ecs34",true));
    EXPECT_FALSE(Tree.Root().AddChild("home/cjnitta/ecs34",true));
    EXPECT_TRUE(Tree.Root().AddChild("home/cjnitta/ecs34/proj1",true));
    EXPECT_TRUE(Tree.Root().AddChild("home/cjnitta/ecs34/proj2",true));
    EXPECT_TRUE(Tree.Root().AddChild("home/cjnitta/ecs36c",true));


    EXPECT_EQ(std::string(Tree), "/\n"
                                 "|--bin\n"
                                 "|--dev\n"
                                 "|--etc\n"
                                 "|--home\n"
                                 "|  `--cjnitta\n"
                                 "|     |--ecs34\n"
                                 "|     |  |--proj1\n"
                                 "|     |  `--proj2\n"
                                 "|     `--ecs36c\n"
                                 "|--proc\n"
                                 "|--usr\n"
                                 "`--var");
}

TEST(FileSystemTree, IterateNodeTest){
    CFileSystemTree Tree;

    EXPECT_TRUE(Tree.Root().AddChild("bin"));
    EXPECT_TRUE(Tree.Root().AddChild("etc"));
    EXPECT_TRUE(Tree.Root().AddChild("home"));
    EXPECT_TRUE(Tree.Root().AddChild("dev"));
    EXPECT_TRUE(Tree.Root().AddChild("proc"));
    EXPECT_TRUE(Tree.Root().AddChild("usr"));
    EXPECT_TRUE(Tree.Root().AddChild("var"));
    EXPECT_TRUE(Tree.Root().AddChild("/home/cjnitta/ecs34",true));
    EXPECT_TRUE(Tree.Root().AddChild("home/cjnitta/ecs34/proj1",true));
    EXPECT_TRUE(Tree.Root().AddChild("home/cjnitta/ecs34/proj2",true));
    EXPECT_TRUE(Tree.Root().AddChild("home/cjnitta/ecs36c",true));

    CFileSystemTree::CEntryIterator Iter = Tree.Root().begin();

    EXPECT_EQ(Iter->Name(), "bin");
    Iter++;
    EXPECT_EQ(Iter->Name(), "dev");
    Iter++;
    EXPECT_EQ(Iter->Name(), "etc");
    Iter++;
    EXPECT_EQ(Iter->Name(), "home");
    ++Iter;
    EXPECT_EQ(Iter->Name(), "proc");
    ++Iter;
    EXPECT_EQ(Iter->Name(), "usr");
    Iter++;
    EXPECT_EQ(Iter->Name(), "var");
    Iter++;
    EXPECT_EQ(Iter, Tree.Root().end());
    Iter--;
    EXPECT_EQ(Iter->Name(), "var");
    Iter--;
    --Iter;
    --Iter;
    EXPECT_EQ(Iter->Name(), "home");

    CFileSystemTree::CEntryIterator Iter2 = Iter->begin()->begin();
    EXPECT_EQ(Iter2->Name(), "ecs34");
    CFileSystemTree::CEntryIterator Iter3 = Iter2->begin();
    EXPECT_EQ(Iter3->Name(), "proj1");
    EXPECT_EQ(Iter3->Parent().Name(), "ecs34");
    Iter3++;
    EXPECT_EQ(Iter3->Name(), "proj2");
    EXPECT_EQ(Iter3->Parent().Name(), "ecs34");
    EXPECT_EQ(Iter3->Parent().Parent().Name(), "cjnitta");
    EXPECT_EQ(Iter3->FullPath(), "/home/cjnitta/ecs34/proj2");
    Iter2++;
    EXPECT_EQ(Iter2->Name(), "ecs36c");
    EXPECT_EQ(Iter2->FullPath(), "/home/cjnitta/ecs36c");
    EXPECT_TRUE(Iter2->AddChild("../../bob",true));
    EXPECT_FALSE(Tree.Root().AddChild("/home/bob",true));
    EXPECT_EQ(Iter2->FullPath(), "/home/cjnitta/ecs36c");
    EXPECT_FALSE(Iter2->AddChild("../../../../bob",true));
}

TEST(FileSystemTree, FindNodeTest){
    CFileSystemTree Tree;

    EXPECT_TRUE(Tree.Root().AddChild("bin"));
    EXPECT_TRUE(Tree.Root().AddChild("etc"));
    EXPECT_TRUE(Tree.Root().AddChild("home"));
    EXPECT_TRUE(Tree.Root().AddChild("dev"));
    EXPECT_TRUE(Tree.Root().AddChild("proc"));
    EXPECT_TRUE(Tree.Root().AddChild("usr"));
    EXPECT_TRUE(Tree.Root().AddChild("var"));
    EXPECT_TRUE(Tree.Root().AddChild("/home/cjnitta/ecs34",true));
    EXPECT_TRUE(Tree.Root().AddChild("home/cjnitta/ecs34/proj1",true));
    EXPECT_TRUE(Tree.Root().AddChild("home/cjnitta/ecs34/proj2",true));
    EXPECT_TRUE(Tree.Root().AddChild("home/cjnitta/ecs36c",true));

    auto NodeIter = Tree.Find("home/cjnitta/ecs34");

    EXPECT_FALSE(NodeIter == Tree.NotFound());
    EXPECT_TRUE(NodeIter != Tree.NotFound());
    EXPECT_EQ(NodeIter->FullPath(), "/home/cjnitta/ecs34");

    EXPECT_EQ(Tree.Find("/a/path/to/nowhere"), Tree.NotFound());
}

TEST(FileSystemTree, AddDataTest){
    CFileSystemTree Tree;

    EXPECT_TRUE(Tree.Root().AddChild("bin"));
    EXPECT_TRUE(Tree.Root().AddChild("etc"));
    EXPECT_TRUE(Tree.Root().AddChild("home"));
    EXPECT_TRUE(Tree.Root().AddChild("dev"));
    EXPECT_TRUE(Tree.Root().AddChild("proc"));
    EXPECT_TRUE(Tree.Root().AddChild("usr"));
    EXPECT_TRUE(Tree.Root().AddChild("var"));
    EXPECT_TRUE(Tree.Root().AddChild("/home/cjnitta/ecs34",true));
    EXPECT_TRUE(Tree.Root().AddChild("home/cjnitta/ecs34/proj1",true));
    EXPECT_TRUE(Tree.Root().AddChild("home/cjnitta/ecs34/proj2",true));
    EXPECT_TRUE(Tree.Root().AddChild("home/cjnitta/ecs36c",true));

    auto NodeIter = Tree.Find("home/cjnitta/ecs34/proj1");

    EXPECT_FALSE(NodeIter == Tree.NotFound());
    EXPECT_TRUE(NodeIter != Tree.NotFound());
    EXPECT_TRUE(NodeIter->SetData({'a', 'b', 'c', 'd'}));
    std::vector< char > TempData;
    EXPECT_TRUE(NodeIter->GetData(TempData));
    EXPECT_EQ(TempData.size(), 4);
    EXPECT_EQ(TempData[0], 'a');
    EXPECT_EQ(TempData[1], 'b');
    EXPECT_EQ(TempData[2], 'c');
    EXPECT_EQ(TempData[3], 'd');

    auto NodeIter2 = Tree.Find("home/cjnitta/ecs34");
    EXPECT_FALSE(NodeIter2->SetData({'e', 'f', 'g', 'h'}));
    EXPECT_FALSE(Tree.Root().AddChild("home/cjnitta/ecs34/proj1/failedchild",true));

}

TEST(FileSystemTree, RenameNodeTest){
    CFileSystemTree Tree;

    EXPECT_TRUE(Tree.Root().AddChild("bin"));
    EXPECT_TRUE(Tree.Root().AddChild("etc"));
    EXPECT_TRUE(Tree.Root().AddChild("home"));
    EXPECT_TRUE(Tree.Root().AddChild("dev"));
    EXPECT_TRUE(Tree.Root().AddChild("proc"));
    EXPECT_TRUE(Tree.Root().AddChild("usr"));
    EXPECT_TRUE(Tree.Root().AddChild("var"));
    EXPECT_TRUE(Tree.Root().AddChild("/home/cjnitta/ecs34",true));
    EXPECT_TRUE(Tree.Root().AddChild("home/cjnitta/ecs34/proj1",true));
    EXPECT_TRUE(Tree.Root().AddChild("home/cjnitta/ecs34/proj2",true));
    EXPECT_TRUE(Tree.Root().AddChild("home/cjnitta/ecs36c",true));

    CFileSystemTree::CEntryIterator Iter = Tree.Root().begin();

    EXPECT_EQ(Iter->Name(), "bin");
    EXPECT_FALSE(Iter->Rename("etc"));
    EXPECT_TRUE(Iter->Rename("hop"));
    Iter = Tree.Root().begin();
    Iter++;
    Iter++;
    Iter++;
    EXPECT_EQ(Iter->Name(), "hop");
    EXPECT_EQ(std::string(Tree), "/\n"
                                 "|--dev\n"
                                 "|--etc\n"
                                 "|--home\n"
                                 "|  `--cjnitta\n"
                                 "|     |--ecs34\n"
                                 "|     |  |--proj1\n"
                                 "|     |  `--proj2\n"
                                 "|     `--ecs36c\n"
                                 "|--hop\n"
                                 "|--proc\n"
                                 "|--usr\n"
                                 "`--var");
 
    auto UsrIter = Tree.Find("/usr");
    auto ECS34Iter = Tree.Find("/home/cjnitta/ecs34");

    EXPECT_FALSE(UsrIter == Tree.NotFound());
    EXPECT_FALSE(ECS34Iter == Tree.NotFound());

    EXPECT_TRUE(UsrIter->SetChild("ecs34", ECS34Iter));
    EXPECT_EQ(std::string(Tree), "/\n"
                                 "|--dev\n"
                                 "|--etc\n"
                                 "|--home\n"
                                 "|  `--cjnitta\n"
                                 "|     `--ecs36c\n"
                                 "|--hop\n"
                                 "|--proc\n"
                                 "|--usr\n"
                                 "|  `--ecs34\n"
                                 "|     |--proj1\n"
                                 "|     `--proj2\n"
                                 "`--var");
}

TEST(FileSystemTree, RemoveNodeTest){
    CFileSystemTree Tree;

    EXPECT_TRUE(Tree.Root().AddChild("bin"));
    EXPECT_TRUE(Tree.Root().AddChild("etc"));
    EXPECT_TRUE(Tree.Root().AddChild("home"));
    EXPECT_TRUE(Tree.Root().AddChild("dev"));
    EXPECT_TRUE(Tree.Root().AddChild("proc"));
    EXPECT_TRUE(Tree.Root().AddChild("usr"));
    EXPECT_TRUE(Tree.Root().AddChild("var"));
    EXPECT_TRUE(Tree.Root().AddChild("/home/cjnitta/ecs34",true));
    EXPECT_TRUE(Tree.Root().AddChild("home/cjnitta/ecs34/proj1",true));
    EXPECT_TRUE(Tree.Root().AddChild("home/cjnitta/ecs34/proj2",true));
    EXPECT_TRUE(Tree.Root().AddChild("home/cjnitta/ecs36c",true));

    CFileSystemTree::CEntryIterator Iter = Tree.Root().begin();

    Iter++;
    Iter++;
    Iter++;

    CFileSystemTree::CEntryIterator Iter2 = Iter->begin();
    EXPECT_FALSE(Iter->RemoveChild("bob"));
    EXPECT_TRUE(Iter->RemoveChild("cjnitta"));
    EXPECT_FALSE(Iter->RemoveChild("cjnitta"));

    EXPECT_EQ(std::string(Tree), "/\n"
                                 "|--bin\n"
                                 "|--dev\n"
                                 "|--etc\n"
                                 "|--home\n"
                                 "|--proc\n"
                                 "|--usr\n"
                                 "`--var");
}
