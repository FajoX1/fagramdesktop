/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/
#include "api/api_common.h"

#include "base/qt/qt_key_modifiers.h"
#include "data/data_histories.h"
#include "data/data_thread.h"
#include "history/history.h"

namespace Api {

SendAction::SendAction(
	not_null<Data::Thread*> thread,
	SendOptions options)
: history(thread->owningHistory())
, options(options)
, replyTo({ .messageId = { history->peer->id, thread->topicRootId() } }) {
	replyTo.topicRootId = replyTo.messageId.msg;
}

SendOptions DefaultSendWhenOnlineOptions() {
	return {
		.scheduled = kScheduledUntilOnlineTimestamp,
		.silent = base::IsCtrlPressed(),
	};
}

MTPInputReplyTo SendAction::mtpReplyTo() const {
	return Data::ReplyToForMTP(history, replyTo);
}

} // namespace Api
