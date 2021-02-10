def in_range_N2S(bbox):

    #only calculate the cars (from south to north) run across the line and not over Y +- calculateRange_y

    x = bbox[0]

    y = bbox[1]

    w = bbox[2]

    h = bbox[3]

    cx = int(x+(w/2))

    cy = int(y+(h/2))

    if(cx>=calculateLine2[0][0] and cx<=calculateLine2[1][0] and \

        cy>calculateLine2[0][1]-calculateRange_y and cy<=(calculateLine2[1][1]+calculateRange_y) ):

        return True

    else:

        return False

if(in_range_S2N(box)==True or in_range_N2S(box)==True):

    now_IDs.append(id)

    now_BBOXES.append(box)

    now_CENTROIDS.append(bbox2Centroid(box))

    now_LABELS.append(labelName)

    now_COUNTED.append(False)

    cv2.rectangle(frame, (box[0], box[1]), (box[0]+box[2], box[1]+box[3]), (0,255,0), 2)