# AAStudy
AA Developer Study Group

# Prepare

## git pull
$ git clone https://github.com/elementalkiss/aastudy /c/work/AAStudy

## sublime - MD file management. (markdown을 관리 할 수 있다면 자기가 사용하고 싶은거 사용하기.)
### download
* http://www.sublimetext.com/2
* Windows 64bit version. https://download.sublimetext.com/Sublime%20Text%202.0.2a%20x64%20Setup.exe

###package control install
* control + ` 콘솔 창을 열고 아래 코드를 삽입 후 엔터

import urllib2,os,hashlib; h = 'df21e130d211cfc94d9b0905775a7c0f' + '1e3d39e33b79698005270310898eea76'; pf = 'Package Control.sublime-package'; ipp = sublime.installed_packages_path(); os.makedirs( ipp ) if not os.path.exists(ipp) else None; urllib2.install_opener( urllib2.build_opener( urllib2.ProxyHandler()) ); by = urllib2.urlopen( 'http://packagecontrol.io/' + pf.replace(' ', '%20')).read(); dh = hashlib.sha256(by).hexdigest(); open( os.path.join( ipp, pf), 'wb' ).write(by) if dh == h else None; print('Error validating download (got %s instead of %s), please try manual install' % (dh, h) if dh != h else 'Please restart Sublime Text to finish installation')

* restart

### install plugin
* control + shift + p에서 Package Control: Install Package 선택.(inst만 치면 나옴.)
* MarkdownEditing
* SmartMarkdown
* Markdown Preview

# Ref

## Markdown CheatSheet (꼭! 한번 보세요!)

* https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet