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
        frame.push_back(pt::ptree::value_type("frame", bboxTree));
        frame.put("rotated", false)
            .put("trimmed", false);
        frame.push_back(pt::ptree::value_type("spriteSourceSize", bboxTree));
        frame.push_back(pt::ptree::value_type("sourceSize", pt::ptree().put("w", bbox.Width).put("h", bbox.Height)));
        frame.push_back(pt::ptree::value_type("pivot", pt::ptree().put("x", 0.5f).put("y", 0.5f)));

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
        frames.push_back(pt::ptree::value_type(std::to_string(frame), CreateFrame(sprite)));
    }

    pt::ptree tree;

    tree.push_back(pt::ptree::value_type("frames", frames));

    pt::write_json(outputFile, tree);
}
