#!/bin/bash
#CA LAB 02

mkdir loop_merge_output

#Look line 6 and 9

valgrind --tool=cachegrind --D1=16384,8,32 --L2=131072,8,64 --cachegrind-out-file=loop_merge_output/loop_merge_out_01 ./loop_merge

valgrind --tool=cachegrind --D1=32768,8,32 --L2=262144,8,64 --cachegrind-out-file=loop_merge_output/loop_merge_out_02 ./loop_merge

valgrind --tool=cachegrind --D1=32768,8,64 --L2=262144,8,64 --cachegrind-out-file=loop_merge_output/loop_merge_out_03 ./loop_merge

cg_annotate loop_merge_output/loop_merge_out_01 ~/lab/source/loop_merge.cpp > loop_merge_output/cg_loop_merge_01

cg_annotate loop_merge_output/loop_merge_out_02 ~/lab/source/loop_merge.cpp > loop_merge_output/cg_loop_merge_02

cg_annotate loop_merge_output/loop_merge_out_03 ~/lab/source/loop_merge.cpp > loop_merge_output/cg_loop_merge_03

mkdir loop_merge_opt_output

#Look line 7 and 9

valgrind --tool=cachegrind --D1=16384,8,32 --L2=131072,8,64 --cachegrind-out-file=loop_merge_opt_output/loop_merge_opt_out_01 ./loop_merge_opt

valgrind --tool=cachegrind --D1=32768,8,32 --L2=262144,8,64 --cachegrind-out-file=loop_merge_opt_output/loop_merge_opt_out_02 ./loop_merge_opt

valgrind --tool=cachegrind --D1=32768,8,64 --L2=262144,8,64 --cachegrind-out-file=loop_merge_opt_output/loop_merge_opt_out_03 ./loop_merge_opt

cg_annotate loop_merge_opt_output/loop_merge_opt_out_01 ~/lab/source/loop_merge_opt.cpp > loop_merge_opt_output/cg_loop_merge_opt_01

cg_annotate loop_merge_opt_output/loop_merge_opt_out_02 ~/lab/source/loop_merge_opt.cpp > loop_merge_opt_output/cg_loop_merge_opt_02

cg_annotate loop_merge_opt_output/loop_merge_opt_out_03 ~/lab/source/loop_merge_opt.cpp > loop_merge_opt_output/cg_loop_merge_opt_03

mkdir access_seq_output

#Look line 9

valgrind --tool=cachegrind --D1=16384,8,32 --L2=131072,8,64 --cachegrind-out-file=access_seq_output/access_seq_out_01 ./access_seq

valgrind --tool=cachegrind --D1=32768,8,32 --L2=262144,8,64 --cachegrind-out-file=access_seq_output/access_seq_out_02 ./access_seq

valgrind --tool=cachegrind --D1=32768,8,64 --L2=262144,8,64 --cachegrind-out-file=access_seq_output/access_seq_out_03 ./access_seq

cg_annotate access_seq_output/access_seq_out_01 ~/lab/source/access_seq.cpp > access_seq_output/cg_access_seq_out_01

cg_annotate access_seq_output/access_seq_out_02 ~/lab/source/access_seq.cpp > access_seq_output/cg_access_seq_out_02

cg_annotate access_seq_output/access_seq_out_03 ~/lab/source/access_seq.cpp > access_seq_output/cg_access_seq_out_03

#Look line 9

mkdir access_strided_output

valgrind --tool=cachegrind --D1=16384,8,32 --L2=131072,8,64 --cachegrind-out-file=access_strided_output/access_strided_out_01 ./access_strided

valgrind --tool=cachegrind --D1=32768,8,32 --L2=262144,8,64 --cachegrind-out-file=access_strided_output/access_strided_out_02 ./access_strided

valgrind --tool=cachegrind --D1=32768,8,64 --L2=262144,8,64 --cachegrind-out-file=access_strided_output/access_strided_out_03 ./access_strided

cg_annotate access_strided_output/access_strided_out_01 ~/lab/source/access_strided.cpp > access_strided_output/cg_access_strided_out_01

cg_annotate access_strided_output/access_strided_out_02 ~/lab/source/access_strided.cpp > access_strided_output/cg_access_strided_out_02

cg_annotate access_strided_output/access_strided_out_03 ~/lab/source/access_strided.cpp > access_strided_output/cg_access_strided_out_03

#Look line 12 and 13

mkdir soa_output

valgrind --tool=cachegrind --D1=16384,8,32 --L2=131072,8,64 --cachegrind-out-file=soa_output/soa_out_01 ./soa

valgrind --tool=cachegrind --D1=32768,8,32 --L2=262144,8,64 --cachegrind-out-file=soa_output/soa_out_02 ./soa

valgrind --tool=cachegrind --D1=32768,8,64 --L2=262144,8,64 --cachegrind-out-file=soa_output/soa_out_03 ./soa

cg_annotate soa_output/soa_out_01 ~/lab/source/soa.cpp > soa_output/cg_soa_out_01

cg_annotate soa_output/soa_out_02 ~/lab/source/soa.cpp > soa_output/cg_soa_out_02

cg_annotate soa_output/soa_out_03 ~/lab/source/soa.cpp > soa_output/cg_soa_out_03

#Look line 12 and 13

mkdir aos_output

valgrind --tool=cachegrind --D1=16384,8,32 --L2=131072,8,64 --cachegrind-out-file=aos_output/aos_out_01 ./aos

valgrind --tool=cachegrind --D1=32768,8,32 --L2=262144,8,64 --cachegrind-out-file=aos_output/aos_out_02 ./aos

valgrind --tool=cachegrind --D1=32768,8,64 --L2=262144,8,64 --cachegrind-out-file=aos_output/aos_out_03 ./aos

cg_annotate aos_output/aos_out_01 ~/lab/source/aos.cpp > aos_output/cg_aos_out_01

cg_annotate aos_output/aos_out_02 ~/lab/source/aos.cpp > aos_output/cg_aos_out_02

cg_annotate aos_output/aos_out_03 ~/lab/source/aos.cpp > aos_output/cg_aos_out_03

#Look line 12 and 14

mkdir product_output

valgrind --tool=cachegrind --D1=16384,8,32 --L2=131072,8,64 --cachegrind-out-file=product_output/product_out_01 ./product

valgrind --tool=cachegrind --D1=32768,8,32 --L2=262144,8,64 --cachegrind-out-file=product_output/product_out_02 ./product

valgrind --tool=cachegrind --D1=32768,8,64 --L2=262144,8,64 --cachegrind-out-file=product_output/product_out_03 ./product

cg_annotate product_output/product_out_01 ~/lab/source/product.cpp > product_output/cg_product_out_01

cg_annotate product_output/product_out_02 ~/lab/source/product.cpp > product_output/cg_product_out_02

cg_annotate product_output/product_out_03 ~/lab/source/product.cpp > product_output/cg_product_out_03

#Look line 18 and 20

mkdir product_block_output

valgrind --tool=cachegrind --D1=16384,8,32 --L2=131072,8,64 --cachegrind-out-file=product_block_output/product_block_out_01 ./product_block 

valgrind --tool=cachegrind --D1=32768,8,32 --L2=262144,8,64 --cachegrind-out-file=product_block_output/product_block_out_02 ./product_block 

valgrind --tool=cachegrind --D1=32768,8,64 --L2=262144,8,64 --cachegrind-out-file=product_block_output/product_block_out_03 ./product_block 

cg_annotate product_block_output/product_block_out_01 ~/lab/source/product_block.cpp > product_block_output/cg_product_block_out_01

cg_annotate product_block_output/product_block_out_02 ~/lab/source/product_block.cpp > product_block_output/cg_product_block_out_02

cg_annotate product_block_output/product_block_out_03 ~/lab/source/product_block.cpp > product_block_output/cg_product_block_out_03
