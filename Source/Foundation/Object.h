//
// Root class for all object hierarchy
//
// Author: Alex V. Boreskoff
//
// Last modified: 9/12/2002
//

#ifndef __OBJECT__
#define __OBJECT__

#ifndef NULL
	#define NULL    0
#endif

#define	OF_DISABLED		0x0001					// flag: object is disabled
#define	OF_DIRTY		0x0002

#pragma warning(disable:4996)
#pragma warning(disable:4244)
#pragma warning(disable:4312)
#pragma warning(disable:4311)
#pragma warning(disable:4267)

class	Encoder;
class	Decoder;
class	String;
/*
class	MetaClass
{
protected:
	const char * className;						// name of the class
	MetaClass  * superClass;					// link to parent metaclass

public:
	MetaClass ( const char * theClassName, MetaClass * super )
	{
		className  = theClassName;
		superClass = super;
	}

	const char * getClassName () const
	{
		return className;
	}

	MetaClass * getSuperClass () const
	{
		return superClass;
	}

	bool	isKindOfClass     ( const MetaClass& theClass ) const;
	bool	isInstanceOfClass ( const MetaClass& theClass ) const;
};*/

class   Object
{
protected:
    char      * name;
    int         refCount;
    long        flags;
    Object    * owner;
	int		    lockCount;

public:
	Object ();
    Object ( const char * aName );
    virtual ~Object ();

    virtual bool isOk () const           // returns non-zero if objects is ok
    {
        return true;
    }

	virtual	bool	isNull () const
	{
		return false;
	}

    virtual int  init ()               // postconstructor initialization
    {
        return 1;
    }

    virtual long hash () const          // return hash for object
    {
        return 0l;
    }

	virtual	int	compare ( const Object * obj ) const;

										// returns  non-zero if this object is
                                        // equal to the object obj
    virtual bool isEqual ( const Object * obj ) const
    {
        return compare ( obj ) == 0;
    }

	virtual	Object * clone () const
	{
		return new Object ( name );
	}

	virtual	void	lock ()
	{
		if ( lockCount++ < 1 )
			doLock ();
	}

	virtual	void	unlock ()
	{
		if ( --lockCount < 1 )
			doUnlock ();
	}

	bool	isLocked () const
	{
		return lockCount > 0;
	}

    int	     release     ();
	Object * autorelease ();

    Object * retain ()
    {
        refCount++;

        return this;
    }

	static	Object * createObject ()
	{
		return new Object;
	}

    const char * getName () const
    {
        return name;
    }

    void    setName ( const char * newName );

    void    setFlag ( int value )
    {
        flags |= value;
    }

    void    clearFlag ( int value )
    {
        flags &= ~value;
    }

    bool testFlag ( int value ) const
    {
        return (flags & value) == value;
    }

    Object * getOwner () const
    {
        return owner;
    }

    void    setOwner ( Object * newOwner );

protected:
    virtual void    doLock   () {}
    virtual void    doUnlock () {}
};

//unsigned long	crc32 ( const void * buf, int size, unsigned long seed = 0xFFFFFFFF );

#endif
