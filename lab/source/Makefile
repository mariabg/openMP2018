CC=g++
CFLAGS=-std=c++14 -g 
BINDIR=bin
PROGS=\
$(BINDIR)/loop_merge \
$(BINDIR)/loop_merge_opt \
$(BINDIR)/aos \
$(BINDIR)/soa \
$(BINDIR)/product \
$(BINDIR)/product_block \
$(BINDIR)/access_strided \
$(BINDIR)/access_seq\

all: $(BINDIR) $(PROGS)

$(BINDIR):
	mkdir $(BINDIR)

$(BINDIR)/loop_merge: loop_merge.cpp
	$(CC) $(CFLAGS) -o $(BINDIR)/loop_merge loop_merge.cpp

$(BINDIR)/loop_merge_opt: loop_merge_opt.cpp
	     $(CC) $(CFLAGS) -o $(BINDIR)/loop_merge_opt loop_merge_opt.cpp
$(BINDIR)/aos: aos.cpp
	     $(CC) $(CFLAGS) -o $(BINDIR)/aos aos.cpp
$(BINDIR)/soa: soa.cpp
	     $(CC) $(CFLAGS) -o $(BINDIR)/soa soa.cpp
$(BINDIR)/access_seq: access_seq.cpp
	     $(CC) $(CFLAGS) -o $(BINDIR)/access_seq access_seq.cpp
$(BINDIR)/access_strided: access_strided.cpp
	     $(CC) $(CFLAGS) -o $(BINDIR)/access_strided access_strided.cpp
$(BINDIR)/product: product.cpp
	     $(CC) $(CFLAGS) -o $(BINDIR)/product product.cpp
$(BINDIR)/product_block: product_block.cpp
	     $(CC) $(CFLAGS) -o $(BINDIR)/product_block product_block.cpp

clean:
	$(RM) $(PROGS)




