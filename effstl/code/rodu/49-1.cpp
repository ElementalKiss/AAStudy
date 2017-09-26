//
// Example from ESTL Item 49
//

#include <string>
#include <vector>
#include <iostream>
#include <list>
#include <map>
#include <iterator>
using namespace std;

class NiftyEmailProgram {
private:
	typedef map<string, string> NicknameMap;

	NicknameMap nicknames;				// map from nicknames to
										// email addresses
public:
    void showEmailAddress(const string& nickname);
    //void showEmailAddress(const string& nickname) const;
};

void NiftyEmailProgram::showEmailAddress(const string& nickname)
{
    //...
    NicknameMap::iterator i = nicknames.find(nickname);
    if (i != nicknames.end())
        cout << "Not found" << endl;
    //...
}

// #2
//void NiftyEmailProgram::showEmailAddress(const string& nickname) const
//{
//	//...
//	NicknameMap::iterator i = nicknames.find(nickname);
//	if (i != nicknames.end())
//		cout << "Not found" << endl;
//	//...
//}
//> e:\workspace\effectivestl\effectivestl\49 - 1.cpp(36) : error C2440 : 'initializing' : cannot convert from 'std::_Tree_const_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const _Kty,_Ty>>>>' to 'std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<const _Kty,_Ty>>>>'
//> with
//> [
//    > _Kty = std::string,
//    > _Ty = std::string
//    > ]
//> e:\workspace\effectivestl\effectivestl\49 - 1.cpp(36) : note: No constructor could take the source type, or constructor overload resolution was ambiguous


int main()
{
	using namespace std;

 // #1
 //string s(10);
 //> e:\workspace\effectivestl\effectivestl\49 - 1.cpp(47) : error C2664 : 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(const std::basic_string<char,std::char_traits<char>,std::allocator<char>> &)' : cannot convert argument 1 from 'int' to 'std::initializer_list<_Elem>'
 //> with
 //> [
 //    > _Elem = char
 //    > ]
 //> e:\workspace\effectivestl\effectivestl\49 - 1.cpp(47) : note: No constructor could take the source type, or constructor overload resolution was ambiguous

 // #3
	//vector<string*> v;						// try to print a container
	//copy(v.begin(), v.end(),				// of string* pointers as
	//	ostream_iterator<string>(cout));	// string objects
 //1 > c:\program files(x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\xutility(2310) : error C2679 : binary '=' : no operator found which takes a right - hand operand of type 'std::basic_string<char,std::char_traits<char>,std::allocator<char>> *' (or there is no acceptable conversion)
 //    1 > c:\program files(x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\iterator(284) : note: could be 'std::ostream_iterator<std::string,char,std::char_traits<char>> &std::ostream_iterator<std::string,char,std::char_traits<char>>::operator =(std::ostream_iterator<std::string,char,std::char_traits<char>> &&)'
 //    1 > c:\program files(x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\iterator(284) : note: or 'std::ostream_iterator<std::string,char,std::char_traits<char>> &std::ostream_iterator<std::string,char,std::char_traits<char>>::operator =(const std::ostream_iterator<std::string,char,std::char_traits<char>> &)'
 //    1 > c:\program files(x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\iterator(255) : note: or 'std::ostream_iterator<std::string,char,std::char_traits<char>> &std::ostream_iterator<std::string,char,std::char_traits<char>>::operator =(const _Ty &)'
 //    1 > with
 //    1 > [
 //        1 > _Ty = std::string
 //            1 > ]
 //    1 > c:\program files(x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\xutility(2310) : note: while trying to match the argument list '(std::ostream_iterator<std::string,char,std::char_traits<char>>, std::basic_string<char,std::char_traits<char>,std::allocator<char>> *)'
 //            1 > c:\program files(x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\xutility(2329) : note: see reference to function template instantiation '_OutIt std::_Copy_unchecked1<_InIt,_OutIt>(_InIt,_InIt,_OutIt,std::_General_ptr_iterator_tag)' being compiled
 //            1 > with
 //            1 > [
 //                1 > _OutIt = std::ostream_iterator<std::string, char, std::char_traits<char>>,
 //                    1 > _InIt = std::basic_string<char, std::char_traits<char>, std::allocator<char>> **
 //                    1 > ]
 //            1 > c:\program files(x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\xutility(2342) : note: see reference to function template instantiation '_OutIt std::_Copy_unchecked<std::basic_string<char,std::char_traits<char>,std::allocator<char>>**,_Iter>(_InIt,_InIt,_OutIt)' being compiled
 //                    1 > with
 //                    1 > [
 //                        1 > _OutIt = std::ostream_iterator<std::string, char, std::char_traits<char>>,
 //                            1 > _Iter = std::ostream_iterator<std::string, char, std::char_traits<char>>,
 //                            1 > _InIt = std::basic_string<char, std::char_traits<char>, std::allocator<char>> **
 //                            1 > ]
 //                    1 > c:\program files(x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\xutility(2351) : note: see reference to function template instantiation '_OutIt std::_Copy_no_deprecate<_InIt,_OutIt>(_InIt,_InIt,_OutIt)' being compiled
 //                            1 > with
 //                            1 > [
 //                                1 > _OutIt = std::ostream_iterator<std::string, char, std::char_traits<char>>,
 //                                    1 > _InIt = std::_Vector_iterator < std::_Vector_val<std::_Simple_types<std::basic_string<char, std::char_traits<char>, std::allocator<char>> *>>>
 //                                    1 > ]
 //                            1 > e:\workspace\effectivestl\effectivestl\49 - 1.cpp(66) : note: see reference to function template instantiation '_OutIt std::copy<std::_Vector_iterator<std::_Vector_val<std::_Simple_types<std::basic_string<char,std::char_traits<char>,std::allocator<char>> *>>>,std::ostream_iterator<std::string,char,std::char_traits<char>>>(_InIt,_InIt,_OutIt)' being compiled
 //                                    1 > with
 //                                    1 > [
 //                                        1 > _OutIt = std::ostream_iterator<std::string, char, std::char_traits<char>>,
 //                                            1 > _InIt = std::_Vector_iterator < std::_Vector_val<std::_Simple_types<std::basic_string<char, std::char_traits<char>, std::allocator<char>> *>>>
 //                                            1 > ]
 //                                    1 > Done building project "EffectiveSTL.vcxproj" --FAILED.

 // #4
	//list<int>::iterator i1, i2;				// pass bidirectional iterators to
	//sort(i1, i2);							// an algorithm requiring random
	//										// access iterators
 //1 > c:\program files(x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\algorithm(2908) : error C2784 : 'unknown-type std::operator -(const std::move_iterator<_RanIt> &,const std::move_iterator<_RanIt2> &)' : could not deduce template argument for 'const std::move_iterator<_RanIt> &' from 'std::_List_unchecked_iterator<std::_List_val<std::_List_simple_types<int>>>'
 //    1 > c:\program files(x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\xutility(2197) : note: see declaration of 'std::operator -'
 //    1 > c:\program files(x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\algorithm(2916) : note: see reference to function template instantiation 'void std::_Sort_unchecked<std::_List_unchecked_iterator<std::_List_val<std::_List_simple_types<int>>>,_Pr>(_RanIt,_RanIt,_Pr &)' being compiled
 //    1 > with
 //    1 > [
 //        1 > _Pr = std::less<void>,
 //            1 > _RanIt = std::_List_unchecked_iterator < std::_List_val<std::_List_simple_types<int>>>
 //            1 > ]
 //    1 > c:\program files(x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\algorithm(2923) : note: see reference to function template instantiation 'void std::sort<_RanIt,std::less<void>>(_RanIt,_RanIt,_Pr)' being compiled
 //            1 > with
 //            1 > [
 //                1 > _RanIt = std::_List_iterator<std::_List_val<std::_List_simple_types<int>>>,
 //                    1 > _Pr = std::less < void>
 //                    1 > ]
 //            1 > e:\workspace\effectivestl\effectivestl\49 - 1.cpp(104) : note: see reference to function template instantiation 'void std::sort<std::_List_iterator<std::_List_val<std::_List_simple_types<int>>>>(_RanIt,_RanIt)' being compiled
 //                    1 > with
 //                    1 > [
 //                        1 > _RanIt = std::_List_iterator < std::_List_val<std::_List_simple_types<int>>>
 //                            1 > ]
 //                    1 > c:\program files(x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\algorithm(2908) : error C2784 : 'unknown-type std::operator -(const std::reverse_iterator<_RanIt> &,const std::reverse_iterator<_RanIt2> &)' : could not deduce template argument for 'const std::reverse_iterator<_RanIt> &' from 'std::_List_unchecked_iterator<std::_List_val<std::_List_simple_types<int>>>'
 //                            1 > c:\program files(x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\xutility(1288) : note: see declaration of 'std::operator -'
 //                            1 > c:\program files(x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\algorithm(2908) : error C2676 : binary '-' : 'std::_List_unchecked_iterator<std::_List_val<std::_List_simple_types<int>>>' does not define this operator or a conversion to a type acceptable to the predefined operator
 //                            1 > c:\program files(x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\algorithm(2908) : error C2672 : '_Sort_unchecked1' : no matching overloaded function found
 //                            1 > c:\program files(x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\algorithm(2908) : error C2780 : 'void std::_Sort_unchecked1(_RanIt,_RanIt,_Diff,_Pr &)' : expects 4 arguments - 3 provided
 //                            1 > c:\program files(x86)\microsoft visual studio\2017\community\vc\tools\msvc\14.10.25017\include\algorithm(2874) : note: see declaration of 'std::_Sort_unchecked1'
 //                            1 > Done building project "EffectiveSTL.vcxproj" --FAILED.

	return 0;
}
