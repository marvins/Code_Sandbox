#include "ogr_feature.h"
#include "ogr_api.h"
#include "ogrsf_frmts.h"
#include "ogr_p.h"

#include <iostream>

using namespace std;

int     nFetchFID = OGRNullFID;
int     bSummaryOnly = FALSE;
char**  papszOptions = NULL;

/**
 * Main Driver Start
*/
int main( int argc, char* argv[] ){
    
    /// Register OGR Drivers
    cout << "Registering OGR Drivers" << endl;
    OGRRegisterAll();

    /// Initialize our data source
    OGRDataSource  *poDS;

    /// Open the file
    cout << "Opening File" << endl;
    poDS = OGRSFDriverRegistrar::Open( argv[1], FALSE );
    if( poDS == NULL )
    {
        printf( "Open failed.\n" );
        exit( 1 );
    }

    // Iterate through layers
    cout << "Iterating through layers" << endl;
    OGRLayer  *poLayer;
    for( int i=0; i<poDS->GetLayerCount(); i++ ){
    
        // grab the layer
        cout << "- Grabbing layer " << i << endl;
        poLayer = poDS->GetLayer(i);
        cout << "- Layer Name: " << poLayer->GetName() << endl;
        cout << endl;

        // looking at features
        OGRFeature *poFeature;
        poLayer->ResetReading();
        while( (poFeature = poLayer->GetNextFeature()) != NULL ){
            
            // pull layer definition
            cout << "- Pulling Layer Definition" << endl;
            OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
            int iField;
        
            for( iField = 0; iField < poFDefn->GetFieldCount(); iField++ ){

                OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );

                if( poFieldDefn->GetType() == OFTInteger )
                    cout << "   -> " << poFeature->GetFieldAsInteger( iField ) << endl;
                else if( poFieldDefn->GetType() == OFTReal )
                    cout << "   -> " << poFeature->GetFieldAsDouble(iField) << endl;
                else if( poFieldDefn->GetType() == OFTString )
                    cout << "   -> " << poFeature->GetFieldAsString(iField) << endl;
                else
                    cout << "   -> " << poFeature->GetFieldAsString(iField) << endl;
            }
        
            // destroy feature
            OGRFeature::DestroyFeature( poFeature );
        }

        // pull geometry
        int nGeomFieldCount = poLayer->GetLayerDefn()->GetFieldCount();
        cout << "- Geometry Field Count: " << nGeomFieldCount << endl;

        cout << "- Geometry: " << OGRGeometryTypeToName( poLayer->GetGeomType() ) << endl;
        cout << "- Feature Count: " << poLayer->GetFeatureCount() << endl;
        

        // get the extent
        OGREnvelope oExt;
        if( poLayer->GetExtent( &oExt, TRUE ) == OGRERR_NONE ){
            cout << "Extent: (" << oExt.MinX << ", " << oExt.MinY << ") - (" << oExt.MaxX << ", " << oExt.MaxY << ")" << endl;
        }
       

        // query the spatial reference system
        char    *pszWKT;
        if( poLayer->GetSpatialRef() == NULL )
            pszWKT = CPLStrdup( "(unknown)" );
        else
        {
            poLayer->GetSpatialRef()->exportToPrettyWkt( &pszWKT );
        }           

        printf( "Layer SRS WKT:\n%s\n", pszWKT );
        CPLFree( pszWKT );

        if( strlen(poLayer->GetFIDColumn()) > 0 )
            printf( "FID Column = %s\n", 
                    poLayer->GetFIDColumn() );

        if( strlen(poLayer->GetGeometryColumn()) > 0 )
            printf( "Geometry Column = %s\n", 
                    poLayer->GetGeometryColumn() );

        OGRFeatureDefn      *poDefn = poLayer->GetLayerDefn();
        for( int iAttr = 0; iAttr < poDefn->GetFieldCount(); iAttr++ )
        {
            OGRFieldDefn    *poField = poDefn->GetFieldDefn( iAttr );

            printf( "%s: %s (%d.%d)\n",
                    poField->GetNameRef(),
                    poField->GetFieldTypeName( poField->GetType() ),
                    poField->GetWidth(),
                    poField->GetPrecision() );
        }

    }

    OGRFeature  *poFeature = NULL;

    if( nFetchFID == OGRNullFID && !bSummaryOnly )
    {
        while( (poFeature = poLayer->GetNextFeature()) != NULL )
        {
            poFeature->DumpReadable( NULL, papszOptions );
            OGRFeature::DestroyFeature( poFeature );
        }
    }
    else if( nFetchFID != OGRNullFID )
    {
        poFeature = poLayer->GetFeature( nFetchFID );
        if( poFeature == NULL )
        {
            printf( "Unable to locate feature id %d on this layer.\n", 
                    nFetchFID );
        }
        else
        {
            poFeature->DumpReadable( NULL, papszOptions );
            OGRFeature::DestroyFeature( poFeature );
        }
    }


    OGRDataSource::DestroyDataSource( poDS );
}
