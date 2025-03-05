#!/bin/bash

sudo apt install wget
wget -qO- https://raw.githubusercontent.com/burhancodes/fagram-deb/main/fagram_repo.asc | sudo tee /etc/apt/trusted.gpg.d/fagram_repo.asc
echo "deb [arch=amd64] https://raw.githubusercontent.com/burhancodes/fagram-deb/main/apt/repo/ bionic main" | sudo tee /etc/apt/sources.list.d/fagram.list
sudo apt update && sudo apt install fagram -y
