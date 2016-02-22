#!/bin/bash

function usage()
{
    echo "usage: dfgame COMMAND ARGS"
}

if [[ $# -lt 1 ]]; then
    echo "dfgame"
    usage
else
    case $1 in
        "new")
            if [[ $# -lt 2 ]]; then
                echo "new: Missing project name"
                usage
            else
                if [[ -e "$2" ]]; then
                    echo -e "\e[1;41mCan't make project: File exists\e[0m"
                    exit 1
                else
                    type="gameOnly"
                    if [[ $# -ge 3 ]]; then
                        type=$3
                    fi
                    if [[ ! -e "/usr/share/dfgame/$type" || ! -d "/usr/share/dfgame/$type" ]]; then
                        echo -e "\e[1;41mCan't make project: Project type $type is invalid\e[0m"
                        exit 1
                    fi

                    echo -e "\e[1;33m[\e[1;32mBuilding Project Directory\e[1;33m]\e[0m"
                    mkdir "$2"
                    shopt -s dotglob nullglob
                    cp -rt "$2" /usr/share/dfgame/$type/*

                    if [[ -e "$2/install.sh" ]]; then
                        echo -e "\e[1;35mTemplate Found\n\e[1;33m[\e[1;32mInstalling Template\e[1;33m]\e[0m"
                        chmod +x "$2/install.sh"
                        cd "$2"
                        "./install.sh" "$2"
                        rm "./install.sh"
                    fi
                    echo -e "\e[1;33m[\e[1;32mCreating Repository\e[1;33m]\e[0m"
                    echo -e "# Ignore the compiled executable\n./$2" >> .gitignore
                    git init
                    git add src/*/*
                    git add config.mk
                    git add makefile
                    git add .gitignore
                    git commit -m "Created an intial dfgame project"
                    echo -e "\e[1;33m[\e[1;32mBuilding Project\e[1;33m]\e[0m"
                    make all
                    echo -e "\e[1;33m[\e[1;32mProject Creation Finished\e[1;33m]\e[0m"
                fi
            fi
            ;;

        "help")
            echo "dfgame"
            usage
            ;;
        *)
            echo "Invalid command: $1"
            usage
            ;;
    esac
fi
