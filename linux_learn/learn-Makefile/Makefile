
# add auto search path
VPATH = add:sub:.

# define value
OBJS = add_int.o add_float.o sub_int.o sub_float.o main.o
OBJSDIR = .objs

# gcc complie options
CFLAGS = -Isub -Iadd -O2

TARGET = run
RM = rm -f

# object complie and link
$(TARGET): $(OBJSDIR) $(OBJS)
	$(CC) -o $@ $(OBJSDIR)/*.o $(CFLAGS)


# pattern match
$(OBJS): %.o:%.c
	$(CC) -c  $(CFLAGS) $< -o $(OBJSDIR)/$@

# create .objs dir
$(OBJSDIR): 
	mkdir -p ./$@

clean:
	$(RM) $(TARGET) $(OBJS)
	$(RM) $(OBJSDIR)/*.o

