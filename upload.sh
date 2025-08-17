#!/bin/bash

git add .

echo "Enter commit message: "
read msg
git commit -m "$msg"

git pull --rebase
git push origin main