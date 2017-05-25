
if(long long|long|short|double|unsigned long| unsigned long long|unsigned short|float)
{
	return 0;
}
if(String|ByteString|DOMString|USVString)
	return ""

if(boolean)
	return true

if (GLboolean)
	return GLboolean value = 0;

if (Comment)
	return Comment::create(*this, "");

if(Attr|HTMLCollection|Element|nodelist o Element|HTMLElement|Node|IDBRequest|SVGTransform|ItemProperty)
	return nullptr

if(BarProp)
	return BarProp::create(nullptr, BarProp::Menubar);

if(DOMMatrix)
	return  new DOMMatrixReadOnly();

if(void)
	return

if(Promise)
	return ScriptPromise()