#ifndef __LOGIN_CODE_MANAGER_H__
#define __LOGIN_CODE_MANAGER_H__
#include "AVLTree.h"

struct CLoginCodeItem
{
    UINT64 uAccountID   = 0;
    UINT64 uEndTime     = 0;
    INT32  nLoginCode   = 0;
};

class CLoginCodeManager : public AVLTree<UINT64, CLoginCodeItem>
{
    CLoginCodeManager();
    ~CLoginCodeManager();
public:
    static CLoginCodeManager* GetInstancePtr();

public:
    INT32       CreateLoginCode(UINT64 uAccountID);

    BOOL        CheckLoginCode(UINT64 uAccountID, INT32 nCode);


};

#endif //__LOGIN_CODE_MANAGER_H__