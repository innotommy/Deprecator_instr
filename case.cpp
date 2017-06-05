
if(long long|long|short|double|unsigned long| unsigned long long|unsigned short|float|GLint|DOMHighResTimeStamp)
{
	return 0;
}
if(String|ByteString|DOMString|USVString|ByteString)
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

if(Promise|Promise<USVString>|Promise<JSON>|Promise<Blob>|Promise<ArrayBuffer>|promise|ScriptPromise)
	return ScriptPromise()
if(AudioParam)
	return new AudioParam(this.context(),0,0,0)|nullptr
if(SVGTransform)
	return SVGTransformTearOff::CreateDetached()|nullptr
if(NodeList)
	return nullptr
if(StaticNodeList)
	return StaticNodeList::createEmpty()
if(MimeType)
	return nullptr
if(IDBKeyRang)
	return nullptr
if(HTMLElement)
	return nullptr|0
if(sequence<MediaStreamTrack>)
	return MediaStreamTrackVector tracks
if(Window)
	return nullptr
if(SVGRect)
	return nullptr|FloatRect()
if(Response)
	return nullptr
if(Range)
	return nullptr
if(PerformanceEntryList)
	return PerformanceEntryVector entries
if(MediaStream)
	return 0|nullptr
if(HTMLTableSectionElement) ?
	return nullptr
if(ClientRect)
	return 0|ClientRect::create()
if(screen) ~~
	return nullptr
if(location)
	return 0| nullptr
if(applicationCache)
	return nullptr
if(XPathResult)
	return nullptr
if(WebGLActiveInfo)
	return nullptr
if(TextTrack)
	return 0 |nullptr
if(Text)
	return nullptr
if(TagCollection)
	return ensureCachedCollection<TagCollection>(TagCollectionType,nullAtom,localName);
if(Selection)
	return nullptr|0
if(Plugin)
	return nullptr
if(PerformanceNavigation) ~
	return nullptr
if(PerformanceTiming) ~
	return nullptr 
if(Navigator)
	return nullptr
if(MediaStreamVector)
	return MediaStreamVector sample
if(MediaQueryList)
	return nullptr
if(ImageData)
	return nullptr
if(IDBOpenDBRequest)
	return nullptr
if(IDBObjectStore)
	return nullptr
if(IDBIndex)
	return nullptr
if(History)
	return nullptr
if(GLenum)
	return 0 |GLenum error
if(Float32Array) ~
	return nullptr
if(DocumentType)
	return nullptr|Member<DocumentType> sample
if(DOMWindow)
	return nullptr
if(ClientRectList)
	return ClientRectList::create()
if(CanvasGradient)
	returnCanvasGradient* gradient = CanvasGradient::create(FloatPoint(0,0), FloatPoint(0,0))|nullptr
if(CSSStyleDeclaration)
	return nullptr|
if(visibilityState)
	return PageVisibilityStateHidden
if(vector)
	return std::vector<rtc::scoped_refptr<RtpSenderInterface>> ret
if(std::vector<rtc::scoped_refptr<RtpReceiverInterface>>)
	return std::vector<rtc::scoped_refptr<RtpReceiverInterface>> ret
if(sequence<unrestricted double>)
	return  Vector<double> sample
if(sequence<USVString>)
	return  Vector<String> result;
if(sequence<MutationRecord>)
	return MutationRecordVector records;
if(sequence<FormDataEntryValue>)
	return HeapVector<FormDataEntryValue> results;
if(sequence<ByteString>)
	return Vector<String>()
if(XPathNSResolver) ~
	return nullptr
if(XPathExpression)
	return XPathExpression::create()
if(XMLDocument)
	return nullptr
if(WebGLUniformLocation)
	return nullptr
if(WebGLTexture)
	return nullptr
if(WebGLShaderPrecisionFormat)
	return nullptr
if(WebGLShader)
	return nullptr
if(WebGLRenderbuffer)
	return nullptr
if(WebGLProgram)
	return nullptr
if(WebGLFramebuffer)
	return nullptr
if(WebGLBuffer)
	return nullptr
if(WaveShaperNode)~~
	return nullptr
if(VideoPlaybackQuality)
	return VideoPlaybackQuality::create(videoElement.document(),0,0,0);
if(Uint8Array)
	return String name = "";
if(USVString)
	return String()
if(TreeWalker)~~
	return nullptr
if(TextTrackCue)
	return nullptr
if(TextMetrics)
	return TextMetrics* metrics = TextMetrics::create();
if(StyleSheet)
	return nullptr
if(StereoPannerNode) ~~
	return nullptr
if(SourceBuffer)
	return 0
if(ScriptProcessorNode) ~
	return nullptr
if(ScriptValue)
	return ScriptValue::createNull(scriptState)
if(SVGNumber)
	return SVGNumberTearOff::create(SVGNumber::create(0.0f), 0,PropertyIsNotAnimVal)
if(SVGLength)
	return SVGLengthTearOff::create(SVGLength::create(), 0, PropertyIsNotAnimVal);
if(SVGAngle)
	return SVGAngleTearOff::create(SVGAngle::create(), 0, PropertyIsNotAnimVal);
if(Request)
	return nullptr
if(RTCDataChannel)
	return nullptr
if(ProcessingInstruction)
	return nullptr
if(PeriodicWave)
	return nullptr|PeriodicWave::create(*this, OMFloat32Array* sample, OMFloat32Array* sample2, false, exceptionState);
if(PannerNode) ~~
	return nullptr
if(OscillatorNode) ~~
	return nullptr
if(Nullable<Vector<String>>)
	return nullptr
if(Nullable<HeapVector<Member<WebGLShader>>>)
	return nullptr
if(NodeIterator) ~~
	return nullptr
if(MimeTypeArray) ~~
	return nullptr
if(MediaStreamAudioSourceNode) ~~
	return nullptr
if(MediaStreamAudioDestinationNode) ~~
	return nullptr
if(MediaKeySession)
	return nullptr
if(MediaElementAudioSourceNode) ~~
	return nullptr
if(MediaDevices)
	return m_mediaDevices
if(ItemPropertyType)
	return ItemProperty* sample
if(IDBTransaction)
	return nullptr
if(HTMLTableRowElement)
	return nullptr
if(HTMLTableCaptionElement)
	return HTMLTableCaptionElement* caption;
if(HTMLOptionElement)
	return nullptr
if(HTMLHeadElement)
	return 0|nullptr
if(HTMLDocument)
	return HTMLDocument::create(init);
if(Geolocation) ~~
	return nullptr
if(GamepadList)
	return GamepadList::create();
if(GainNode)
	return nullptr
if(Float64Array)
	return DOMFloat64Array::create(double array[], 16)
if(File)
	return 0|nullptr
if(Event)
	return nullptr
if(DynamicsCompressorNode) ~~
	return nullptr
if(DelayNode) ~~
	return nullptr
if(DOMString[])
	return Vector<String> languages;
if(DOMSelection)
	return nullptr
if(DOMImplementation) ~~
	return nullptr
if(ConvolverNode) ~~
	return nullptr
if(Comment)
	return Comment::create(*this,"");
if(ChannelSplitterNode) ~~
	return nullptr
if(ChannelMergerNode) ~~
	return nullptr
if(CanvasPattern)
	return nullptr
if(CanPlayTypeResult)
	return emptyString();
if(CSSValue&)
	return HeapVector<Member<const CSSValue>, 4> sample;
if(CSSValue)
	return nullptr
if(CSSRule)
	return nullptr
if(CDATASection)
	return nullptr
if(Blob)
	return nullptr
if(BiquadFilterNode) ~~
	return nullptr
if(AudioBufferSourceNode)
	return AudioBufferSourceNode* sample;
if(AudioBuffer)
	return AudioBuffer* sample 
if(ArrayBufferView)
	return nullptr
if(AnalyserNode) ~~
	return nullptr
if(SVGMatrix)
	return SVGMatrixTearOff* matrix = create(value());|nullptr
if(SVGMatrixTearOff)
	return SVGMatrixTearOff::create(AffineTransform());
if(AffineTransform)
	return AffineTransform sample;
if(Document)
	return nullptr|
if(DocumentFragment)
	return nullptr|  DocumentFragment* fragment = nullptr;|DocumentFragment::create(document());
if(SVGPoint) ~~
	return nullptr