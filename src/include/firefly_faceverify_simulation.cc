#include "./firefly_faceverify.hpp"
#include "../types/firefly_face_sdk.hpp"

#ifndef FOR_ARM

FF_API FF_Detector firefly_fd_init_detector() { return nullptr; }

FF_API void firefly_fd_deinit_detector(const FF_Verifier detector_handle) {}

FF_API int firefly_fd_detect_image(
	const FF_Detector detector_handle,
	const FF_Image image,
	FF_FaceInfo **p_faces_array,
	int *p_faces_count) {
		return FF_E_JUST_A_SIMULATION;
	}

FF_API void firefly_fd_free_detect_result(const FF_FaceInfo* faces_array) {}

FF_API int firefly_fd_get_version() { return 196608; }

FF_API FF_Verifier firefly_fv_init_verifier() { return nullptr; }

FF_API void firefly_fv_deinit_verifier(const FF_Verifier verifier_handle) { }

FF_API int firefly_fv_extract_feature(
	const FF_Verifier verifier_handle,
	const FF_Image image,
	const FF_FaceInfo face,
	FF_FaceFeatures* p_feature) {
		return FF_E_JUST_A_SIMULATION;
	}

FF_API float firefly_fv_compare_feature(
	FF_FaceFeatures& feature1,
	FF_FaceFeatures& feature2) {
		return 0.0f;
	}

FF_API int firefly_fv_get_feature_length(const FF_Verifier verifier) {
	return 0;
}

FF_API int firefly_fv_get_version() { return 196608; }

#endif
