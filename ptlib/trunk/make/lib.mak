
ifeq ($(P_SHAREDLIB),1)

ifndef LIBVER_MAJ
LIBVER_MAJ	= 1
endif

ifndef LIBVER_MIN
LIBVER_MIN	= 0
endif

ifndef LIBVER_PAT
LIBVER_PAT	= 0
endif

LIBNAME_MAJ		= $(LIB_BASENAME).$(LIBVER_MAJ)
LIBNAME_MIN		= $(LIBNAME_MAJ).$(LIBVER_MIN)
LIBNAME_PAT		= $(LIBNAME_MIN).$(LIBVER_PAT)

$(LIBDIR)/$(LIB_BASENAME): $(LIBDIR)/$(LIBNAME_PAT)
	@rm -f $(LIBDIR)/$(LIB_BASENAME)
	ln -s $(LIBDIR)/$(LIBNAME_PAT) $(LIBDIR)/$(LIB_BASENAME)
	@rm -f $(LIBDIR)/$(LIBNAME_MAJ)
	ln -s $(LIBDIR)/$(LIBNAME_PAT) $(LIBDIR)/$(LIBNAME_MAJ)
	@rm -f $(LIBDIR)/$(LIBNAME_MIN)
	ln -s $(LIBDIR)/$(LIBNAME_PAT) $(LIBDIR)/$(LIBNAME_MIN)

$(LIBDIR)/$(LIBNAME_PAT): $(OBJS)
	@if [ ! -d $(LIBDIR) ] ; then mkdir $(LIBDIR) ; fi
	gcc -shared -Wl,-soname,$(LIB_BASENAME).1 -o $(LIBDIR)/$(LIBNAME_PAT) $(OBJS)

CLEAN_FILES += $(LIBDIR)/$(LIBNAME_PAT) $(LIBDIR)/$(LIB_BASENAME) $(LIBDIR)/$(LIBNAME_MAJ) $(LIBDIR)/$(LIBNAME_MIN)

else

$(LIBDIR)/$(LIB_BASENAME): $(OBJS)
	@if [ ! -d $(LIBDIR) ] ; then mkdir $(LIBDIR) ; fi
ifdef RANLIB
	ar rc $(LIBDIR)/$(LIB_BASENAME) $(OBJS)
	ranlib $(LIBDIR)/$(LIB_BASENAME)
else
	ar rcs $(LIBDIR)/$(LIB_BASENAME) $(OBJS)
endif

CLEAN_FILES += $(LIBDIR)/$(LIB_BASENAME)

endif

