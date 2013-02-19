#!/bin/sh

# ANSI Color Script
. ~/.scripts/initializeANSI.sh

# Create aliases to archive functions
alias rm="~/.scripts/newrm.sh"
alias unrm="~/.scripts/unrm.sh"
alias clearrm="~/.scripts/clearrm.sh"
alias lsd="~/.scripts/formatls.sh"
alias rm-manager="~/.scripts/rm-manager.sh"

#  Create task manager utilities
alias task-manager="${HOME}/.scripts/task-manager"
alias create-task="~/.scripts/task-manager -c"
alias delete-task="~/.scripts/task-managery -d"
alias list-tasks="~/.scripts/task-manager -l"
alias view-task="~/.scripts/task-manager -v"
alias clear-tasks="~/.scripts/task-manager -x"

# GS2-specific alias'
alias gs2-assets="~/.scripts/gs2-iptool.sh"

