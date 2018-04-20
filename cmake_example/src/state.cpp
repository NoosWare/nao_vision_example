#include "state.hpp"

nao_state::nao_state()
: detecting_faces__(noos_platform::noos,
                    std::bind(&nao_state::face_callback, this, std::placeholders::_1))
{}

state nao_state::new_state()
{
    cv::Mat pic;
    get_image()(robot_ip::ip, pic);
    detecting_faces__.send(pic);
    return state__;
}

void nao_state::face_callback(std::vector<noos::object::face> faces)
{
    std::cout << "Found: " << faces.size() << " faces" << std::endl; 
    if (faces.size() != 0) { 
        int i = 0;
        int max_size = 0;
        for(int j = 0; j < faces.size(); j++) {
            auto size = faces.at(j).bottom_right_x - faces.at(j).top_left_x;
            if (size > max_size) {
                max_size = size;
                i = j;
            }
        }
        state__.center_face_x = faces.at(i).top_left_x + max_size/2;
        face_location()(state__.center_face_x,
                        state__.angle_head,
                        state__.movement_time);
            //head__.move(angle_head__, time_move);
        state__.face_found = true;
        state__.state_time = boost::chrono::system_clock::now();
    }
    std::cout << "Angle at the end: " << state__.angle_head << std::endl;

}
