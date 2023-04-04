# 防爆栈!!! -fsplit-stack
for num in {01..14}
do 
    cd ../${num}.*
    printf "============= Directory: %s =============\n" `pwd`
    rm data/*
    printf ">> compile:"; time `g++ generator.cpp -fconcepts -fsplit-stack -o gen; g++ jury.cpp -Wall -fsplit-stack -O2 -o jury`
    printf ">> generator:"; time ./gen
    printf ">> jury:"; time `ls -1 data | while read file; do ./jury < "./data/"${file} > "./data/"${file%.in}".out"; done`
    rm ./gen ./jury
done
rm ../09.整向量/data/*.out
cd ../work

echo "================== zip data =================="
for num in {01..14}
do
    cd ../${num}.*
    printf "============= Directory: %s =============\n" `pwd`
    cd data
    zip -r ../data.zip *
    cd ..
    rm -rf data
done
cd ../work
