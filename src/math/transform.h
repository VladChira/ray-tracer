#pragma once
#include "utilities.h"

namespace raytracer
{
    class Transform
    {
    public:
        Transform()
        {
            m = Eigen::Matrix4f::Identity();
            inv_m = Eigen::Matrix4f::Identity();
        }

        Transform(const Eigen::Matrix4f matrix)
        {
            m = matrix;
            inv_m = matrix.inverse();
        }

        Transform(const Eigen::Matrix4f &matrix, const Eigen::Matrix4f &inv_matrix)
        {
            m = matrix;
            inv_m = inv_matrix;
        }

        Transform operator*(const Transform &t2) const
        {
            return Transform(m * t2.m, t2.inv_m * inv_m);
        }

        static Transform Inverse(const Transform &t)
        {
            return Transform(t.inv_m, t.m);
        }

        static Transform Transpose(const Transform &t)
        {
            return Transform(t.m.transpose(), t.inv_m.transpose());
        }

        static Transform Translate(const Eigen::Vector3f &delta_v)
        {
            Eigen::Matrix4f mm;
            mm << 1, 0, 0, delta_v.x(),
                0, 1, 0, delta_v.y(),
                0, 0, 1, delta_v.z(),
                0, 0, 0, 1;
            return Transform(mm, mm.inverse());
        }

        static Transform Scale(const Eigen::Vector3f &scale_v)
        {
            Eigen::Matrix4f mm;
            mm << scale_v.x(), 0, 0, 0,
                0, scale_v.y(), 0, 0,
                0, 0, scale_v.z(), 0,
                0, 0, 0, 1;
            return Transform(mm, mm.inverse());
        }

        static Transform RotateX(const float theta_deg)
        {
            Eigen::Matrix4f mm;
            float sin_theta = std::sin(deg_to_rad(theta_deg));
            float cos_theta = std::cos(deg_to_rad(theta_deg));
            mm << 1, 0, 0, 0,
                0, cos_theta, -sin_theta, 0,
                0, sin_theta, cos_theta, 0,
                0, 0, 0, 1;
            // The rotation matrix is orthogonal, so A^-1 = A^T
            return Transform(mm, mm.transpose());
        }

        static Transform RotateY(const float theta_deg)
        {
            Eigen::Matrix4f mm;
            float sin_theta = std::sin(deg_to_rad(theta_deg));
            float cos_theta = std::cos(deg_to_rad(theta_deg));
            mm << cos_theta, 0, sin_theta, 0,
                0, 1, 0, 0,
                -sin_theta, 0, cos_theta, 0,
                0, 0, 0, 1;
            // The rotation matrix is orthogonal, so A^-1 = A^T
            return Transform(mm, mm.transpose());
        }

        static Transform RotateZ(const float theta_deg)
        {
            Eigen::Matrix4f mm;
            float sin_theta = std::sin(deg_to_rad(theta_deg));
            float cos_theta = std::cos(deg_to_rad(theta_deg));
            mm << cos_theta, -sin_theta, 0, 0,
                sin_theta, cos_theta, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1;
            // The rotation matrix is orthogonal, so A^-1 = A^T
            return Transform(mm, mm.transpose());
        }

        Eigen::Vector3f transform_point(const Eigen::Vector3f &p)
        {
            // std::cout << p << "\n\n";
            Eigen::Transform<float, 3, Eigen::Affine> affineTransform(m);
            return affineTransform * p;
        }

        Eigen::Vector3f transform_vector(const Eigen::Vector3f &v)
        {
            Eigen::Transform<float, 3, Eigen::Affine> affineTransform(m);
            return affineTransform.linear() * v;
            // Eigen::Vector3f tv;
            // tv << m(0, 0) * v.x() + m(0, 1) + v.y(), m(0, 2) + v.z(),
            //     m(1, 0) * v.x() + m(1, 1) * v.y() + m(1, 2) * v.z(),
            //     m(2, 0) * v.x() + m(2, 1) * v.y() + m(2, 2) * v.z();
            // return tv;
        }

        Eigen::Vector3f transform_normal(const Eigen::Vector3f &n)
        {
            Eigen::Transform<float, 3, Eigen::Affine> affineTransform(m);
            Eigen::Matrix3f normal_matrix = affineTransform.linear().inverse().transpose();
            return (normal_matrix * n).normalized();
            // Eigen::Vector3f tn;
            // tn << inv_m(0, 0) * n.x() + inv_m(1, 0) * n.y() + inv_m(2, 0) * n.z(),
            //       inv_m(0, 1) * n.x() + inv_m(1, 1) * n.y() + inv_m(2, 1) * n.z(),
            //       inv_m(0, 2) * n.x() + inv_m(1, 2) * n.y() + inv_m(2, 2) * n.z();
            // return tn.normalized();
        }

        Eigen::AlignedBox3f transform_bounding_box(const Eigen::AlignedBox3f &b)
        {
            // Initialize an array to store the transformed corner points
            Eigen::Vector3f corners[8];

            // Transform each of the eight corners
            for (int i = 0; i < 8; ++i)
            {
                Eigen::Vector3f corner = b.corner((Eigen::AlignedBox3f::CornerType)i);
                corners[i] = this->transform_point(corner);
            }

            // Compute the minimum and maximum coordinates of the transformed corner points
            Eigen::Vector3f minCoords = corners[0];
            Eigen::Vector3f maxCoords = corners[0];

            for (int i = 1; i < 8; ++i)
            {
                minCoords = minCoords.cwiseMin(corners[i]);
                maxCoords = maxCoords.cwiseMax(corners[i]);
            }

            // Reconstruct the transformed bounding box
            Eigen::AlignedBox3f tb(minCoords, maxCoords);
            return tb;
        }

    private:
        Eigen::Matrix4f m, inv_m;
    };
}