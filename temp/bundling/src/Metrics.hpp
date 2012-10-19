#ifndef __SRC_METRICS_HPP__
#define __SRC_METRICS_HPP__

#include <deque>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

class SceneID{

    public:
        
        SceneID( );
        SceneID( const int& sn, const int& camid );
        SceneID( const int& sn, const std::deque<int>& camids );

        bool operator  < ( const SceneID& rhs )const;
        bool operator  > ( const SceneID& rhs )const;
        bool operator == ( const SceneID& rhs )const;
        bool operator != ( const SceneID& rhs )const;

        bool complete;

        int scene_number;
        std::deque<int> camera_idx_list;


};

std::ostream& operator << ( std::ostream& ostr, const SceneID& id );

class Metrics{

    public:
        
        Metrics();

        void append_scene_list( std::vector<SceneID> const& scene_list );
        
        void validate_scene_list( const int& camera_count );
        
        std::vector<std::pair<std::string,int> > query_failures_by_camera()const;
        std::vector<std::pair<int,std::vector<std::string> > > query_failures_by_scene( )const;

        void merge( const Metrics& rhs );

        std::set<SceneID> scene_list;

        std::vector<std::string> camera_names;

        int number_complete_frames;
        int number_incomplete_frames;
        int number_total_frames;

        int camera_cnt;
        
        bool validated;
};



std::ostream& operator << ( std::ostream& ostr, const Metrics& metrics );

#endif

