#ifndef ROOT_H
#define ROOT_H

class Root {
public:
    static unsigned requiredBlocks(unsigned containerBlocks);

    // Initialize empty Root file directory for the given container size
    void init(unsigned containerBlocks);

private:
};

#endif // ROOT_H