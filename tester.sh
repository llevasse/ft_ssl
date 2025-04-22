#!/bin/bash

total=0;
correct_md5=0;
correct_sha256=0;


RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

for arg in "$@";
do
  ((total += 1))
  echo "TEST MD5-SHA256 FOR \"$arg\""

  out1="$(echo -n "$arg" | ./ft_ssl md5 | cut -d ' ' -f2)"
  out2="$(echo -n "$arg" | openssl md5 | cut -d ' ' -f2)"
  
  if [ "${out1}" != "${out2}" ]; then
    echo -e "${RED}"
    echo "ft_ssl  md5 ${out1}"
    echo "openssl md5 ${out2}"
    echo -n -e "${NC}"
  else
    echo -e "${GREEN}"
    echo -n "md5 okay : ${out1}"
    echo -n -e "${NC}"
    ((correct_md5 += 1))
  fi

  out1="$(echo -n "$arg" | ./ft_ssl sha256 | cut -d ' ' -f2)"
  out2="$(echo -n "$arg" | openssl sha256 | cut -d ' ' -f2)"
  if [ "${out1}" != "${out2}" ]; then
    echo -e "${RED}"
    echo "ft_ssl  sha256 ${out1}"
    echo "openssl sha256 ${out2}"
    echo -n -e "${NC}"
  else
    echo -e "${GREEN}"
    echo -n "sha256 okay : ${out1}"
    echo -n -e "${NC}"
    ((correct_sha256 += 1))
  fi
  echo ""
  echo ""
done

echo "Total test : ${total}"
echo "Correct md5 : ${correct_md5}"
echo "Correct sha256 : ${correct_sha256}"