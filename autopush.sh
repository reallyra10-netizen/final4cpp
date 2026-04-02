#!/bin/bash 
message="$1"
if [ -z "$message" ]; then 
    echo "Usage is : bash $0 <commit> " 
    exit 1 
fi 

git add .
git commit -m "$message"
git push -u origin "$(git branch --show-current)"