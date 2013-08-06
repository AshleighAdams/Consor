#!/bin/bash




PS3='Please enter your choice: '
options=("GNU Make" "Code::Blocks")
toolset="gmake" 

select opt in "${options[@]}"
do
    case $opt in
        "GNU Make")
            toolset="gmake"
            break
            ;;
        "Code::Blocks")
            toolset="codeblocks"
            break
            ;;
    esac
done

# clean existing projects
ThirdParty/Premake4.elf --file=Consor.lua clean
ThirdParty/Premake4.elf --file=Consor.lua $toolset
