// ConsoleApplication9.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <vector>

int main()
{
    std::vector<bool> boolContainer;
    //bool* pb = &boolContainer[0];
    /*
    �ɰ���	�ڵ�	����	������Ʈ	����	��	��ǥ�� ����(Suppression) ����
    ����	C2440	'�ʱ�ȭ ��': 'std::_Vb_reference<std::_Wrap_alloc<std::allocator<_Other>>> *'���� 'bool *'(��)�� ��ȯ�� �� �����ϴ�. 
    */

    auto autoPb = &boolContainer[0];
    // std::_Vb_iter_base<std::_Wrap_alloc<std::allocator<unsigned int> > >	{_Myptr=0x00000000 {???} _Myoff=0 }	std::_Vb_iter_base<std::_Wrap_alloc<std::allocator<unsigned int> > >

    return 0;
}

