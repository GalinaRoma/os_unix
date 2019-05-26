# Array for store the game field.
STATE=(' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ' ')

# Draw game field from state.
draw_state() {
    tput clear
    for i in {1..9}; do
        if ((($i%3) == 0 )); then
            echo "${STATE[i-1]}"
            if (($i != 9)); then
                echo "—————"
            fi
        else echo -n "${STATE[i-1]}|"
        fi
    done
}

# Change the cell with pos index if it is empty.
change_symbol() {
    while [[ ${STATE[$pos-1]} != ' ' ]]; do
        echo "This cell is not empty!"
        wait_pos
    done
    change_state $pos $1
}

# Wait for position from user.
wait_pos() {
    echo -n "Enter the position (1-9) for empty cell: "
    read -n 1 pos
    while ! [[ $pos =~ [1-9] ]]; do
        echo
        echo -n "Enter number! You: "
        read -n 1 pos
    done
    echo
}

# Send position of current user to game enemy.
send_changes() {
    echo "$pos" | nc localhost 36000
}

# Wait changes from game enemy.
wait_changes() {
    new_pos=`nc -l -p 36000 -w 1`
}

# Change the state by $1 position to $2 element.
change_state() {
    STATE[$1-1]=$2
}

#Check the draw end or the winner combinations.
check_end_game() {
    if check_winner $1; then
        if [[ "$1" == "$2" ]]; then
            echo "You win!"
        else
            echo "You lose!"
        fi
        exit
    fi
    for i in {1..9}; do
        if [[ ${STATE[i-1]} == ' ' ]]; then
            return 1
        fi
    done
    echo "Draw!"
    exit
}

# Check the equalless of three cells.
is_equal() {
    if [[ "${STATE[$1]}" == "${STATE[$2]}" ]] && [[ "${STATE[$1]}" == "${STATE[$3]}" ]] && [[ "${STATE[$1]}" == "$4" ]]; then
        return 0
    fi
    return 1
}

# Check the winner combinations. $1 - the gamer element (O/X)
check_winner() {
    if is_equal 0 3 6 $1 || is_equal 1 4 7 $1 || is_equal 2 5 8 $1 || is_equal 0 1 2 $1 || is_equal 3 4 5 $1 || is_equal 6 7 8 $1 || is_equal 0 4 8 $1 || is_equal 2 4 6 $1; then
        return 0
    fi
    return 1
}

# Main cycle for game.
main() {
    if [[ $1 == 'O' ]]; then
        while true; do
            draw_state
            wait_pos
            change_symbol 'O'
            draw_state
            send_changes
            check_end_game 'O' $1
            wait_changes
            change_state $new_pos 'X'
            check_end_game 'X' $1
        done
    elif [[ $1 == 'X' ]]; then
        while true; do
            draw_state
            wait_changes
            change_state $new_pos 'O'
            draw_state
            check_end_game 'O' $1
            wait_pos
            change_symbol 'X'
            draw_state
            send_changes
            check_end_game 'X' $1
        done
    else
        echo "Give the 'O' or 'X' symbol as parameter to game"
    fi
}

main $1