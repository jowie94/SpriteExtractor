#include "Paper2DSerializer.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Paper2DSerializer
{
    boost::property_tree::ptree SerializeBBox(const BBox& bbox)
    {
        namespace pt = boost::property_tree;

        pt::ptree bboxTree;

        bboxTree.put("x", bbox.X);
        bboxTree.put("y", bbox.Y);
        bboxTree.put("w", bbox.Width);
        bboxTree.put("h", bbox.Height);

        return bboxTree;
    }

    boost::property_tree::ptree CreateFrame(const BBox& bbox)
    {
        namespace pt = boost::property_tree;

        pt::ptree frame;

        pt::ptree bboxTree = SerializeBBox(bbox);
        frame.put_child("frame", bboxTree);
        frame.put("rotated", false);
        frame.put("trimmed", false);
        frame.put_child("spriteSourceSize", bboxTree);
        
        frame.put("sourceSize.w", bbox.Width);
        frame.put("sourceSize.h", bbox.Height);

        frame.put("pivot.x", 0.5f);
        frame.put("pivot.y", 0.5f);

        return frame;
    }
}

void Paper2DSerializer::Serialize(const std::string& outputFile, const SpriteExtractor::SpriteList& spriteList)
{
    namespace pt = boost::property_tree;

    pt::ptree frames;

    int frame = 0;
    for (const auto& sprite : spriteList)
    {
        frames.put_child(std::to_string(frame), CreateFrame(sprite));
        ++frame;
    }

    pt::ptree tree;

    tree.put_child("frames", frames);

    pt::write_json(outputFile, tree);
}
