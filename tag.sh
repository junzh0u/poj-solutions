#!/bin/zsh

if [ -z "$1" ]; then
  printf "All Tags:\n"
  find solutions -name 'tags' -type d -exec ls {} \; | sort | uniq
else
  problem_dir="solutions/$1"
  if [ -d "$problem_dir" ]; then
    if [ -z "$2" ]; then
      if [ -d "$problem_dir/tags" ]; then
        printf "Tags of $1:\n"
        ls "$problem_dir/tags"
      else
        printf "$1 has no tags."
      fi
    else
      printf "Tagging $1 with $2 ...\n"
      mkdir "$problem_dir/tags" &> /dev/null
      touch "$problem_dir/tags/$2"
    fi
  else
    printf "Problem $1 not found.\n"
  fi
fi
