#!/bin/bash


# Path to log files
# INFO_LOG="./Log/2025-01-06-13-28-43.txt"
# ERROR_LOG="./Log/2025-01-06-13-28-43.txt"

LOG_FILE="./Log/firehorn.logs"
SERIAL_INPUT_LOG="./Log/packets.logs"


# Session and window names
SESSION_NAME="log_monitor"
WINDOW_NAME="logs"

# Start a new tmux session
tmux new-session -d -s $SESSION_NAME -n $WINDOW_NAME

# Create bottom pane
tmux split-window -v -t $SESSION_NAME
tmux select-pane -t 0

# Pane 1: Tail INFO_LOG
tmux send-keys -t $SESSION_NAME "tail -f -n 20 $LOG_FILE | sed -n '/\[INFO\]/,/,$/p'" C-m

# Split the window horizontally and run tail on ERROR_LOG in the second pane
tmux split-window -h -t $SESSION_NAME
tmux send-keys -t $SESSION_NAME "tail -f -n 20 $LOG_FILE | sed -n '/\[ERROR\]/,/,$/p'" C-m

# tmux split-window -v -t $SESSION_NAME

# Make the bottom pane span the entire width
# tmux resize-pane -t 2 -y 10 # Adjust height as needed

tmux select-pane -t 2
# Run any command in the bottom pane (or leave it for manual use)
tmux send-keys -t 2 'cd build' C-m

tmux split-window -h -t $SESSION_NAME
tmux send-keys -t 3 'cd build' C-m
# Focus back on the top-left pane
tmux select-pane -t 0
tmux split-window -h -t $SESSION_NAME
tmux send-keys -t $SESSION_NAME "tail -f -n 20 $SERIAL_INPUT_LOG | sed -n '/\[INFO\]/,/,$/p'" C-m
# Attach to the tmux session
tmux attach-session -t $SESSION_NAME

