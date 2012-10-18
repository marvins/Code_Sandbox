#include "Metrics.hpp"

#include <algorithm>

using namespace std;


class Fail_Cam_Func{

    public:

        bool operator() ( const pair<string,int>& a, const pair<string,int>& b ){
            return a.second > b.second;
        }
};


SceneID::SceneID( const int& sn, const int& camid ){
     
    scene_number = sn;
    camera_idx_list.push_back(camid);

}

bool SceneID::operator  < ( const SceneID& rhs )const{
    return (scene_number < rhs.scene_number );
}

bool SceneID::operator  > ( const SceneID& rhs )const{
    return (scene_number > rhs.scene_number );
}

bool SceneID::operator == ( const SceneID& rhs )const{
    return (scene_number == rhs.scene_number );
}

bool SceneID::operator != ( const SceneID& rhs )const{
    return (scene_number != rhs.scene_number );

}

std::ostream& operator << ( ostream& ostr, const SceneID& id ){

    ostr << "SceneID:  " << id.scene_number << endl;
    ostr << "Matches:  " << id.camera_idx_list.size() << endl;
    return ostr;

}


Metrics::Metrics( ){

    //default everything to zero
    number_complete_frames   = 0;
    number_incomplete_frames = 0;
    number_total_frames      = 0;

}
       

/**
 *  Append a list of scenes to the currently existing list
*/
void Metrics::append_scene_list( vector<SceneID> const& scenes ){
    
    // iterate through each scene
    for( size_t i=0; i<scenes.size(); i++ ){

        //check to see if it already exists
        if( scene_list.find(scenes[i]) != scene_list.end() )
            throw string("Unexpected condition");
        
        //otherwise add the scene 
        scene_list.insert(scenes[i]);

    }


}

void Metrics::validate_scene_list( const int& camera_count ){
    
    // set all counters
    number_total_frames = scene_list.size();
    number_complete_frames = 0;
    number_incomplete_frames = 0;
    
    camera_cnt = camera_count;

    set<SceneID> new_scene_list;

    //iterate through each scene and check if the counts are good
    for( set<SceneID>::iterator it = scene_list.begin(); it != scene_list.end(); it++ ){
        
        if( (int)it->camera_idx_list.size() == camera_count ){
            
            SceneID tempID = *it;
            tempID.complete = true;
            new_scene_list.insert(tempID);
            number_complete_frames++;
        }
        else{
            
            SceneID tempID = *it;
            tempID.complete = false;
            new_scene_list.insert(tempID);
            number_incomplete_frames++;
        }
    }
    
    //swap scene lists
    scene_list = new_scene_list;
}

vector<pair<string,int> > Metrics::query_failures_by_camera()const{

    //failure list
    vector<pair<string,int> > failure_list( camera_cnt);

    //build names into list
    for( size_t i=0; i<failure_list.size(); i++ ){
        failure_list[i].first  = camera_names[i];
        failure_list[i].second = 0;
    }

    //iterate over the scenes, finding missing frames
    for( set<SceneID>::iterator it = scene_list.begin(); it != scene_list.end(); it++ ){
        
        //search through the camera list, incrementing all positive frames
        for( size_t i=0; i<it->camera_idx_list.size(); i++ ){
            failure_list[it->camera_idx_list[i]].second++;
        }
    }

    //negate this list to find the failures
    for( size_t i=0; i<failure_list.size(); i++ )
        failure_list[i].second = scene_list.size() - failure_list[i].second;
    
    //sort by count
    stable_sort( failure_list.begin(), failure_list.end(), Fail_Cam_Func() );


    return failure_list;
}


/**
 * Merge the results from the new metrics
 * object with the previous results.
*/
void Metrics::merge( Metrics const& new_metrics ){

    // make sure the camera counts are uniform
    if( camera_cnt != 0 && camera_cnt != new_metrics.camera_cnt )
        throw string("ERROR: cameras have different counts.");

    // merge the frame counts
    for( set<SceneID>::iterator it=new_metrics.scene_list.begin(); it != new_metrics.scene_list.end(); it++ ){
        
        //check if the new item is already present.  THIS SHOULD NOT HAPPEN AND MUST BE CAUGHT
        if( scene_list.find( *it ) != scene_list.end() ){
            cout << *it << endl;
            throw string("ERROR: Duplicate scenes found, NOT ALLOWED.");
        }

    }

    //union the sets
    scene_list.insert( new_metrics.scene_list.begin(), new_metrics.scene_list.end());


}



std::ostream& operator << ( std::ostream& ostr, const Metrics& metrics ){

    ostr << "Metrics Results" << endl;
    ostr << "---------------" << endl;
    ostr << "Number Complete   Frame Sets: " << metrics.number_complete_frames << endl;
    ostr << "Number Incomplete Frame Sets: " << metrics.number_incomplete_frames << endl;
    ostr << "Total Number of Frame Sets  : " << metrics.number_total_frames;
    ostr << endl;
    ostr << "Camera Failures by Group" << endl;
    ostr << "------------------------" << endl;
    
    vector<pair<string,int> > failure_list = metrics.query_failures_by_camera();
    for( size_t i=0; i<failure_list.size(); i++ ){
        if( failure_list[i].second <= 0 )
            break;

        cout << failure_list[i].first << " has " << failure_list[i].second << " failures" << endl;
    }

    return ostr;
}

