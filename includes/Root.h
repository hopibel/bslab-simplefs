#ifndef ROOT_H
#define ROOT_H

class Root {
public:
    static int requiredBlocks(int containerBlocks);

    // Initialize empty Root file directory for the given container size
    void init(int containerBlocks);

private:
};

#endif // ROOT_H