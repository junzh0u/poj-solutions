#!/bin/zsh

if [ -z $1 ]; then
  printf "All Tags:\n"
  find . -name 'tags' -type d -not -path '*/.git/*' -exec ls {} \; | sort | uniq
else
  if [ -d $1 ]; then
    if [ -z $2 ]; then
      if [ -d $1/tags ]; then
        printf "Tags of $1:\n"
        ls $1/tags
      else
        printf "$1 has no tags."
      fi
    else
      printf "Tagging $1 with $2 ...\n"
      mkdir $1/tags &> /dev/null
      touch $1/tags/$2
    fi
  else
    printf "Problem $1 not found.\n"
  fi
fi
