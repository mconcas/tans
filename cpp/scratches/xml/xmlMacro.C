#if !defined (__CINT__) || defined (__MAKECINT__)
#include <TXMLEngine.h>
#include <TSystem.h>
#endif

void xMacro( const TString filename="books.xml" ) {
   // TXMLEngine
   TXMLEngine *engineptr=new TXMLEngine();
   XMLDocPointer_t docxmlptr=engineptr->ParseFile(filename.Data());

   // Index node positioning on the root element.
   XMLNodePointer_t indexnodeptr=engineptr
      ->DocGetRootElement( docxmlptr );
   // Printing all data.
   Printf( "Document file opened: %s",filename.Data() );
   Printf( "root node: %s",engineptr->GetNodeName( indexnodeptr ) );
   indexnodeptr=engineptr->GetChild( indexnodeptr );
   Printf( "child node name: %s",
      engineptr->GetNodeName( indexnodeptr ) );
   Printf( "\tchild node first attribute: %s=%s",
      engineptr->GetAttrName(engineptr->GetFirstAttr(indexnodeptr)),
      engineptr->GetAttrValue(engineptr->GetFirstAttr(indexnodeptr)));
   // Again.
   indexnodeptr=engineptr->GetChild( indexnodeptr );
   Printf( "child node name: %s",
      engineptr->GetNodeName( indexnodeptr ) );
   Printf( "\t\tchild node first attribute: %s=%s",
      engineptr->GetAttrName(engineptr->GetFirstAttr(indexnodeptr)),
      engineptr->GetAttrValue(engineptr->GetFirstAttr(indexnodeptr)));
   Printf( "\t\tchild node content: %s",
      engineptr->GetNodeContent( indexnodeptr ) );
}
