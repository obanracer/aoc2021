echo "running day $1, puzzle $2..."
cd $1
cd $2
gcc solution.c -o solution
./solution
echo "done."