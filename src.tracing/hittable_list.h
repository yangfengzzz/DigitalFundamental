//
//  hittable_list.h
//  DigitalRender
//
//  Created by 杨丰 on 2020/11/2.
//

#ifndef hittable_list_h
#define hittable_list_h

#include "hittable.h"
#include "aabb.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }
    
    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }
    
    virtual bool hit(const ray& r, double t_min,
                     double t_max, hit_record& rec) const override;
    
    virtual bool bounding_box(double time0, double time1,
                              aabb& output_box) const override;
    
    virtual double pdf_value(const vec3 &o, const vec3 &v) const override;
    virtual vec3 random(const vec3 &o) const override;
    
public:
    std::vector<shared_ptr<hittable>> objects;
};

#endif /* hittable_list_h */
