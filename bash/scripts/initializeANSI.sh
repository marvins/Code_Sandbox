#!/bin/sh

# ANSI Color -- Use these variables to make output in different
# colors and formats. Color names that end with 'f' are forground (text)
# colors and those ending with 'b' are background colors

INITIALIZEANSI_INSTALLED="1"

initializeANSI( ){

    esc="\E" # If this doesn't work, enter an ESC directly

    # Foreground Colors
    blackf="${esc}[30m";  redf="${esc}[31m";   greenf="${esc}[32m"
    yellowf="${esc}[33m"; bluef="${esc}[34m";  purplef="${esc}[35m"
    cyanf="${esc}[36m";   whitef="${esc}[37m";

    # Background Colors
    blackb="${esc}[40m";  redb="${esc}[41m";   greenb="${esc}[42m"
    yellowb="${esc}[43m"; blueb="${esc}[44m";  purpleb="${esc}[45m"
    cyanb="${esc}[46m";   whiteb="${esc}[47m";

    # Special Character Conditions
    boldon="${esc}[1m";     boldoff="${esc}[22m"
    italicson="${esc}[3m";  italicsoff="${esc}[23m"
    ulon="${esc}[4m";       uloff="${esc}[24m"
    invon="${esc}[7m";      invoff="${esc}[27m"

    # Reset to default configuration
    reset="${esc}[0m"
    
}

testANSI( ){

    echo -e "Testing ANSI Color Sequences"
    echo 
    echo -e "Foreground Color Sequences"
    echo -e "${redf} Red Foreground"
    echo -e "${greenf} Green Foreground"
    echo -e "${blackf} Black Foreground"
    echo -e "${yellowf} Yellow Foreground"
    echo -e "${bluef} Blue Foreground"
    echo -e "${purplef} Purple Foreground"
    echo -e "${cyanf} Cyan Foreground"
    echo -e "${whitef} White Foreground${reset}"
    echo 
    echo -e "Background Color Sequences"
    echo -e "${blackb}${whitef} Black Background${reset}"
    echo -e "${redb}${whitef} Red Background${reset}"
    echo -e "${greenb}${whitef} Green Background${reset}"
    echo -e "${yellowb}${whitef} Yellow Background${reset}"
    echo -e "${blueb}${whitef} Blue Background${reset}"
    echo -e "${purpleb}${whitef} Purple Background${reset}"
    echo -e "${cyanb}${whitef} Cyan Background${reset}"
    echo -e "${whiteb}${blackf} White Background${reset}"
    echo
    echo -e "Special Character Conditions"
    echo -e "${boldon} Bold On, ${boldoff} Bold Off ${reset}"
    echo -e "${italicson} Italics On, ${italicsoff} Italics Off ${reset}"
    echo -e "${ulon} Underline On, ${uloff}Underline Off ${reset}"
    echo -e "${invon} Invert Color On, ${invoff}Invert Color Off ${reset}"
    echo 
    echo -e "${reset} Default Color Configuration"

}

initializeANSI
