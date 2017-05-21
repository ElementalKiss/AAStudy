// ConsoleApplication9.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <vector>

int main()
{
    std::vector<bool> boolContainer;
    //bool* pb = &boolContainer[0];
    /*
    심각도	코드	설명	프로젝트	파일	줄	비표시 오류(Suppression) 상태
    오류	C2440	'초기화 중': 'std::_Vb_reference<std::_Wrap_alloc<std::allocator<_Other>>> *'에서 'bool *'(으)로 변환할 수 없습니다. 
    */

    auto autoPb = &boolContainer[0];
    // std::_Vb_iter_base<std::_Wrap_alloc<std::allocator<unsigned int> > >	{_Myptr=0x00000000 {???} _Myoff=0 }	std::_Vb_iter_base<std::_Wrap_alloc<std::allocator<unsigned int> > >

    return 0;
}

