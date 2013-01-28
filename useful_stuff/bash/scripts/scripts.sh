#!/bin/sh

# ANSI Color Script
. ~/.scripts/initializeANSI.sh

# Create aliases to archive functions
alias rm="~/.scripts/newrm.sh"
alias unrm="~/.scripts/unrm.sh"
alias clearrm="~/.scripts/clearrm.sh"
alias lsd="~/.scripts/formatls.sh"
alias rm-manager="~/.scripts/rm-manager.sh"
alias task-manager="${HOME}/.scripts/task-manager.py"
alias create-task="~/.scripts/task-manager.py -c"
alias delete-task="~/.scripts/task-manager.py -d"
alias list-tasks="~/.scripts/task-manager.py -l"
alias view-task="~/.scripts/task-manager.py -v"
alias clear-tasks="~/.scripts/task-manager.py -x"

# GS2-specific alias'
alias gs2-assets="~/.scripts/gs2-iptool.sh"

