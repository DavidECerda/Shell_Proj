#ifndef FILESYSTEMTREE_H
#define FILESYSTEMTREE_H

#include <string>
#include <iterator>
#include <memory>
#include <vector>

///--------------------------------------------------------------
/// @ brief A represention of a file system tree using 
/// shared and weak pointers to connect the nodes which
/// represent files or directories. Shared pointers point 
/// down to children of a node and those children have weak
/// pointers to the parent. This allows nodes to be deleted 
/// and anything connected to that node downstream will be
/// deleted as well.
///--------------------------------------------------------------

class CFileSystemTree{
    protected:
        struct SImplementation;
        std::unique_ptr< SImplementation > DImplementation;

    public:
        class CEntryIterator;
        class CConstEntryIterator;

        class CEntry{
            friend class CFileSystemTree;
            friend class CEntryIterator;
            friend class CConstEntryIterator;
            protected:
                struct SImplementation;
                std::unique_ptr < SImplementation > DImplementation;
            public:
                CEntry();
                CEntry(const CEntry &entry);
                ~CEntry();

                CEntry &operator=(const CEntry &entry);

                bool Valid() const;
                std::string Name() const;
                std::string FullPath() const;

                std::string ToString() const;
                operator std::string() const;

                bool Rename(const std::string &name);

                size_t ChildCount() const;
                bool SetChild(const std::string &name, CEntryIterator &iter);
                bool AddChild(const std::string &path, bool addall=false);
                bool RemoveChild(const std::string &path);

                bool SetData(const std::vector< char > &data);
                bool GetData(std::vector< char > &data) const;

                CEntry &Parent();
                const CEntry &Parent() const;

                CEntryIterator Find(const std::string &name);
                CConstEntryIterator Find(const std::string &name) const;
                CEntryIterator begin();
                CConstEntryIterator begin() const;
                CConstEntryIterator cbegin() const;
                CEntryIterator end();
                CConstEntryIterator end() const;
                CConstEntryIterator cend() const;
        };

        class CEntryIterator{
            friend class CFileSystemTree;
            friend class CEntry;
            friend class CConstEntryIterator;
            protected:
                struct SImplementation;
                std::unique_ptr< SImplementation > DImplementation;

            public:
                CEntryIterator();
                CEntryIterator(const CEntryIterator &iter);
                ~CEntryIterator();

                CEntryIterator& operator=(const CEntryIterator  &iter);
                bool operator==(const CEntryIterator &iter) const;
                bool operator!=(const CEntryIterator &iter) const;

                CEntryIterator& operator++();
                CEntryIterator operator++(int);
                CEntryIterator& operator--();
                CEntryIterator operator--(int);

                CEntry &operator*() const;
                CEntry *operator->() const;
        };

        class CConstEntryIterator{
            friend class CFileSystemTree;
            protected:
                struct SImplementation;
                std::unique_ptr< SImplementation > DImplementation;

            public:
                CConstEntryIterator();
                CConstEntryIterator(const CConstEntryIterator &iter);
                CConstEntryIterator(const CEntryIterator &iter);
                ~CConstEntryIterator();

                CConstEntryIterator& operator=(const CConstEntryIterator &iter);
                bool operator==(const CConstEntryIterator &iter) const;
                bool operator!=(const CConstEntryIterator &iter) const;

                CConstEntryIterator& operator++();
                CConstEntryIterator operator++(int);
                CConstEntryIterator& operator--();
                CConstEntryIterator operator--(int);

                const CEntry &operator*() const;
                const CEntry *operator->() const;
        };

        CFileSystemTree();
        CFileSystemTree(const CFileSystemTree &tree);
        ~CFileSystemTree();

        CFileSystemTree &operator=(const CFileSystemTree &tree);

        CEntry &Root();
        const CEntry &Root() const;

        std::string ToString() const;
        operator std::string() const;

        CEntryIterator Find(const std::string &path);
        CConstEntryIterator Find(const std::string &path) const;

        CEntryIterator NotFound();
        CConstEntryIterator NotFound() const;

};

#endif
