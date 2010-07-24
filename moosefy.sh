#!/bin/bash

function process()
{
    
    if [ "$1" == "Makefile.am" ]; then
        sed -i -e 's/Phoenix/Moose/g' -e 's/PHOENIX/MOOSE/g' $1
    elif ! [ -L $1 ] && [ -e $1 ];then
#        echo "$1 is not symlink."
        name_new=Moose${1##Phoenix} 
        git mv $1 $name_new
        sed -i -e 's/Phoenix/Moose/g' -e 's/PHOENIX/MOOSE/g' $name_new
    elif [ -e $1 ]; then
        name_target=Moose${1##Phoenix} 
        name_src=${name_target%%.cpp}.mm
        git mv $1 $name_target
        ln -sf $name_src $name_target
#        echo "$1 is SYMLINK"
    fi
}

for i in Phoenix*.{h,cpp,mm} Makefile.am; do process $i;done
#for i in Phoenix*.{h,cpp,mm}; do ;done
#for i in *.{h,cpp,mm} Makefile.am;do ;done



