/*
This file is part of FAgram Desktop,
the unofficial desktop client based on Telegram Desktop.

For license and copyright information please follow this link:
https://github.com/fajox1/fagramdesktop/blob/master/LEGAL
*/

#include "telegram_helpers.h"

#include "fa/lang/fa_lang.h"
#include "fa/settings/fa_settings.h"

#include "core/application.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtNetwork/QNetworkRequest>
#include <QtCore/QEventLoop>
#include <QtCore/QElapsedTimer>
#include <QtCore/QDebug>

std::unordered_set<ID> fagram_channels;
std::unordered_set<ID> fagram_devs;

std::unordered_map<ID, bool> state;

void fetchAndParseData(const QUrl &url, std::unordered_set<ID> &targetSet) {
    QNetworkAccessManager manager;
    QNetworkRequest request(url);

    QEventLoop loop;
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
        if (jsonResponse.isArray()) {
            QJsonArray jsonArray = jsonResponse.array();
            for (const QJsonValue &value : jsonArray) {
                if (value.isDouble()) {
                    targetSet.insert(static_cast<ID>(value.toDouble()));
                }
            }
        }
    }

    reply->deleteLater();
}

void initialize_fagram_data() {
    QUrl channelsUrl("https://fagram.fajox.one/channels");
    fetchAndParseData(channelsUrl, fagram_channels);

    QUrl devsUrl("https://fagram.fajox.one/devs");
    fetchAndParseData(devsUrl, fagram_devs);
}

void markAsOnline(not_null<Main::Session*> session) {
	state[session->userId().bare] = true;
}

// stole from ayugram
int getMediaSizeBytes(not_null<HistoryItem*> message) {
	if (!message->media()) {
		return -1;
	}

	const auto media = message->media();

	const auto document = media->document();
	const auto photo = media->photo();

	int64 size = -1;
	if (document) {
		// any file
		size = document->size;
	} else if (photo && photo->hasVideo()) {
		// video
		size = photo->videoByteSize(Data::PhotoSize::Large);
		if (size == 0) {
			size = photo->videoByteSize(Data::PhotoSize::Small);
		}
		if (size == 0) {
			size = photo->videoByteSize(Data::PhotoSize::Thumbnail);
		}
	} else if (photo && !photo->hasVideo()) {
		// photo
		size = photo->imageByteSize(Data::PhotoSize::Large);
		if (size == 0) {
			size = photo->imageByteSize(Data::PhotoSize::Small);
		}
		if (size == 0) {
			size = photo->imageByteSize(Data::PhotoSize::Thumbnail);
		}
	}

	return size;
}

// stole from Ayugram
void readMentions(base::weak_ptr<Data::Thread> weakThread) {
	const auto thread = weakThread.get();
	if (!thread) {
		return;
	}
	const auto peer = thread->peer();
	const auto topic = thread->asTopic();
	const auto rootId = topic ? topic->rootId() : 0;
	using Flag = MTPmessages_ReadMentions::Flag;
	peer->session().api().request(MTPmessages_ReadMentions(
		MTP_flags(rootId ? Flag::f_top_msg_id : Flag()),
		peer->input,
		MTP_int(rootId)
	)).done([=](const MTPmessages_AffectedHistory &result)
	{
		const auto offset = peer->session().api().applyAffectedHistory(
			peer,
			result);
		if (offset > 0) {
			readMentions(weakThread);
		} else {
			peer->owner().history(peer)->clearUnreadMentionsFor(rootId);
		}
	}).send();
}

// stole from Ayugram
void readReactions(base::weak_ptr<Data::Thread> weakThread) {
	const auto thread = weakThread.get();
	if (!thread) {
		return;
	}
	const auto topic = thread->asTopic();
	const auto peer = thread->peer();
	const auto rootId = topic ? topic->rootId() : 0;
	using Flag = MTPmessages_ReadReactions::Flag;
	peer->session().api().request(MTPmessages_ReadReactions(
		MTP_flags(rootId ? Flag::f_top_msg_id : Flag(0)),
		peer->input,
		MTP_int(rootId)
	)).done([=](const MTPmessages_AffectedHistory &result)
	{
		const auto offset = peer->session().api().applyAffectedHistory(
			peer,
			result);
		if (offset > 0) {
			readReactions(weakThread);
		} else {
			peer->owner().history(peer)->clearUnreadReactionsFor(rootId);
		}
	}).send();
}

ID getBareID(not_null<PeerData*> peer) {
    return peerIsUser(peer->id)
               ? peerToUser(peer->id).bare
               : peerIsChat(peer->id)
                     ? peerToChat(peer->id).bare
                     : peerIsChannel(peer->id)
                           ? peerToChannel(peer->id).bare
                           : peer->id.value;
}

bool isFAgramRelated(ID peerId) {
    if (fagram_channels.empty() || fagram_devs.empty()) {
        initialize_fagram_data();
    }
    return fagram_devs.contains(peerId) || fagram_channels.contains(peerId);
}

QString getLocationDC(int dc_id) {
    QString dc_location;

    switch (dc_id) {
        case 1:
            dc_location = "Miami FL, USA";
            break;
        case 2:
            dc_location = "Amsterdam, NL";
            break;
        case 3:
            dc_location = "Miami FL, USA";
            break;
        case 4:
            dc_location = "Amsterdam, NL";
            break;
        case 5:
            dc_location = "Singapore, SG";
            break;
        default:
            dc_location = "UNKNOWN";
            break;
    }

    return dc_location;
}

QString getPeerDC(not_null<PeerData*> peer) {
    int dc = 0;

    if (const auto statsDcId = peer->owner().statsDcId(peer)) {
        dc = statsDcId;
    }
    else if (peer->hasUserpic()) {
        dc = v::match(
            peer->userpicLocation().file().data,
            [&](const StorageFileLocation &data) {
                return data.dcId();
            },
            [&](const WebFileLocation &) {
                return 4;
            },
            [&](const GeoPointLocation &) {
                return 0;
            },
            [&](const AudioAlbumThumbLocation &) {
                return 0;
            },
            [&](const PlainUrlLocation &) {
                return 4;
            },
            [&](const InMemoryLocation &) {
                return 0;
            }
        );
    }
    else {
        return QString("DC_UNKNOWN");
    }

    QString dc_location = getLocationDC(dc);

    if (dc < 1) {
        return QString("DC_UNKNOWN");
    }

    return QString("DC%1, %2").arg(dc).arg(dc_location);
}

QString getDCbyID(int dc) {
    QString dc_location = getLocationDC(dc);

    if (dc < 1) {
        return QString("DC_UNKNOWN");
    }

    return QString("DC%1, %2").arg(dc).arg(dc_location);
}

QString getOnlyDC(not_null<PeerData*> peer) {
    int dc = 0;

    if (const auto statsDcId = peer->owner().statsDcId(peer)) {
        dc = statsDcId;
    }
    else if (peer->hasUserpic()) {
        dc = v::match(
            peer->userpicLocation().file().data,
            [&](const StorageFileLocation &data) {
                return data.dcId();
            },
            [&](const WebFileLocation &) {
                return 4;
            },
            [&](const GeoPointLocation &) {
                return 0;
            },
            [&](const AudioAlbumThumbLocation &) {
                return 0;
            },
            [&](const PlainUrlLocation &) {
                return 4;
            },
            [&](const InMemoryLocation &) {
                return 0;
            }
        );
    }
    else {
        return QString("DC_UNKNOWN");
    }

    if (dc < 1) {
        return QString("DC_UNKNOWN");
    }

    return QString("Datacenter %1").arg(dc);
}

QString getIpDC(int dc_id, bool test) {
    QString ip;

    if (!test) {
        switch (dc_id) {
            case 1:
                ip = "149.154.175.53";
                break;
            case 2:
                ip = "149.154.167.51";
                break;
            case 3:
                ip = "149.154.175.100";
                break;
            case 4:
                ip = "149.154.167.91";
                break;
            case 5:
                ip = "91.108.56.130";
                break;
            default:
                return QString("UNKNOWN");
        }
    }

    else {
        switch (dc_id) {
            case 1:
                ip = "149.154.175.10";
                break;
            case 2:
                ip = "149.154.167.40";
                break;
            case 3:
                ip = "149.154.175.117";
                break;
            default:
                return QString("UNKNOWN");
        }
    }

    return ip;
}

void cleanDebugLogs() {
    QString workingDir = cWorkingDir();
    QDir debugLogsDir(cWorkingDir() + "/DebugLogs");

    if (!debugLogsDir.exists()) {
        return;
    }

    QStringList files = debugLogsDir.entryList(QDir::Files);
    
    for (const QString &file : files) {
        debugLogsDir.remove(file);
    }

    return;
}

bool is_me(ID userId) {
    for (const auto &accountWithIndex : Core::App().domain().accounts()) {
        if (const auto *account = accountWithIndex.account.get()) {
            if (const auto *session = account->maybeSession()) {
                if (session->userId().bare == userId) {
                    return true;
                }
            }
        }
    }
    return false;
}

QString getMediaSize(not_null<HistoryItem*> message) {
	const auto size = getMediaSizeBytes(message);

	if (size == -1) {
		return {};
	}

	return Ui::FormatSizeText(size);
}

QString getMediaMime(not_null<HistoryItem*> message) {
	if (!message->media()) {
		return {};
	}

	const auto media = message->media();

	const auto document = media->document();
	const auto photo = media->photo();

	if (document) {
		// any file
		return document->mimeString();
	} else if (photo && photo->hasVideo()) {
		// video
		return "video/mp4";
	} else if (photo && !photo->hasVideo()) {
		// photo
		return "image/jpeg";
	}

	return {};
}

QString getMediaName(not_null<HistoryItem*> message) {
	if (!message->media()) {
		return {};
	}

	const auto media = message->media();

	const auto document = media->document();

	if (document) {
		return document->filename();
	}

	return {};
}

QString getMediaResolution(not_null<HistoryItem*> message) {
	if (!message->media()) {
		return {};
	}

	const auto media = message->media();

	const auto document = media->document();
	const auto photo = media->photo();

	const auto formatQSize = [=](QSize size)
	{
		if (size.isNull() || size.isEmpty() || !size.isValid()) {
			return QString();
		}

		return QString("%1x%2").arg(size.width()).arg(size.height());
	};

	if (document) {
		return formatQSize(document->dimensions);
	} else if (photo) {
		auto result = photo->size(Data::PhotoSize::Large);
		if (!result.has_value()) {
			result = photo->size(Data::PhotoSize::Small);
		}
		if (!result.has_value()) {
			result = photo->size(Data::PhotoSize::Thumbnail);
		}
		return result.has_value() ? formatQSize(result.value()) : QString();
	}

	return {};
}

QString getMediaDC(not_null<HistoryItem*> message) {
	if (!message->media()) {
		return {};
	}

	const auto media = message->media();

	const auto document = media->document();
	const auto photo = media->photo();

	if (document) {
		return getDCbyID(document->getDC());
	} else if (photo) {
		return getDCbyID(photo->getDC());
	}

	return {};
}

// thanks ayugram
void MessageDetails(not_null<Ui::PopupMenu*> menu, HistoryItem *item) {
    bool show_message_details = FASettings::JsonSettings::GetBool("fa_show_message_details");
    if (!show_message_details) {
        return;
    }

	if (item->isLocal()) {
		return;
	}

	const auto view = item->mainView();
	const auto forwarded = item->Get<HistoryMessageForwarded>();
	const auto views = item->Get<HistoryMessageViews>();
	const auto media = item->media();

	const auto isSticker = (media && media->document() && media->document()->sticker()) ? true : false;

	const auto emojiPacks = HistoryView::CollectEmojiPacks(item, HistoryView::EmojiPacksSource::Message);
	auto containsSingleCustomEmojiPack = emojiPacks.size() == 1;
	if (!containsSingleCustomEmojiPack && emojiPacks.size() > 1) {
		const auto author = emojiPacks.front().id >> 32;
		auto sameAuthor = true;
		for (const auto &pack : emojiPacks) {
			if (pack.id >> 32 != author) {
				sameAuthor = false;
				break;
			}
		}

		containsSingleCustomEmojiPack = sameAuthor;
	}

	const auto isForwarded = forwarded && !forwarded->story && forwarded->psaType.isEmpty();

	const auto messageId = QString::number(item->id.bare);
	const auto messageDate = base::unixtime::parse(item->date());
	const auto messageEditDate = base::unixtime::parse(view ? view->displayedEditDate() : TimeId(0));

	const auto messageForwardedDate =
		isForwarded && forwarded
			? base::unixtime::parse(forwarded->originalDate)
			: QDateTime();

	const auto
		messageViews = item->hasViews() && item->viewsCount() > 0 ? QString::number(item->viewsCount()) : QString();
	const auto messageForwards = views && views->forwardsCount > 0 ? QString::number(views->forwardsCount) : QString();

	const auto mediaSize = media ? getMediaSize(item) : QString();
	const auto mediaMime = media ? getMediaMime(item) : QString();
	// todo: bitrate (?)
	const auto mediaName = media ? getMediaName(item) : QString();
	const auto mediaResolution = media ? getMediaResolution(item) : QString();
	const auto mediaDC = media ? getMediaDC(item) : QString();

	const auto hasAnyPostField =
		!messageViews.isEmpty() ||
		!messageForwards.isEmpty();

	const auto hasAnyMediaField =
		!mediaSize.isEmpty() ||
		!mediaMime.isEmpty() ||
		!mediaName.isEmpty() ||
		!mediaResolution.isEmpty() ||
		!mediaDC.isEmpty();

	const auto callback = Ui::Menu::CreateAddActionCallback(menu);

	callback(Window::PeerMenuCallback::Args{
		.text = FAlang::Translate(QString("fa_message_details")),
		.handler = nullptr,
		.icon = &st::menuIconInfo,
		.fillSubmenu = [&](not_null<Ui::PopupMenu*> menu2)
		{
			if (hasAnyPostField) {
				if (!messageViews.isEmpty()) {
					menu2->addAction(Ui::ContextActionWithSubText(
						menu2->menu(),
						st::menuIconShowInChat,
						FAlang::Translate(QString("fa_message_details_views")),
						messageViews
					));
				}

				if (!messageForwards.isEmpty()) {
					menu2->addAction(Ui::ContextActionWithSubText(
						menu2->menu(),
						st::menuIconViewReplies,
						FAlang::Translate(QString("fa_message_details_share")),
						messageForwards
					));
				}

				menu2->addSeparator();
			}

			menu2->addAction(Ui::ContextActionWithSubText(
				menu2->menu(),
				st::menuIconInfo,
				QString("ID"),
				messageId
			));

			menu2->addAction(Ui::ContextActionWithSubText(
				menu2->menu(),
				st::menuIconSchedule,
				FAlang::Translate(QString("fa_message_details_date")),
				formatDateTime(messageDate)
			));

			if (view && view->displayedEditDate()) {
				menu2->addAction(Ui::ContextActionWithSubText(
					menu2->menu(),
					st::menuIconEdit,
					FAlang::Translate(QString("fa_message_details_edit_date")),
					formatDateTime(messageEditDate)
				));
			}

			if (isForwarded) {
				menu2->addAction(Ui::ContextActionWithSubText(
					menu2->menu(),
					st::menuIconTTL,
					FAlang::Translate(QString("fa_message_details_forwarded_date")),
					formatDateTime(messageForwardedDate)
				));
			}

			if (media && hasAnyMediaField) {
				menu2->addSeparator();

				if (!mediaSize.isEmpty()) {
					menu2->addAction(Ui::ContextActionWithSubText(
						menu2->menu(),
						st::menuIconDownload,
						FAlang::Translate(QString("fa_message_details_filesize")),
						mediaSize
					));
				}

				if (!mediaMime.isEmpty()) {
					const auto mime = Core::MimeTypeForName(mediaMime);

					menu2->addAction(Ui::ContextActionWithSubText(
						menu2->menu(),
						st::menuIconShowAll,
						FAlang::Translate(QString("fa_message_details_type")),
						mime.name()
					));
				}

				if (!mediaName.isEmpty()) {
					auto const shortified = mediaName.length() > 20 ? "…" + mediaName.right(20) : mediaName;

					menu2->addAction(Ui::ContextActionWithSubText(
						menu2->menu(),
						st::menuIconInfo,
						FAlang::Translate(QString("fa_message_details_filename")),
						shortified,
						[=]
						{
							QGuiApplication::clipboard()->setText(mediaName);
						}
					));
				}

				if (!mediaResolution.isEmpty()) {
					menu2->addAction(Ui::ContextActionWithSubText(
						menu2->menu(),
						st::menuIconStats,
						FAlang::Translate(QString("fa_message_details_resoultion")),
						mediaResolution
					));
				}

				if (!mediaDC.isEmpty()) {
					menu2->addAction(Ui::ContextActionWithSubText(
						menu2->menu(),
						st::menuIconBoosts,
						FAlang::Translate(QString("fa_message_details_datacenter")),
						mediaDC
					));
				}

				if (isSticker) {
					auto authorId = getUserIdFromPackId(media->document()->sticker()->set.id);

					if (authorId != 0) {
						menu2->addAction(Ui::ContextActionStickerAuthor(
							menu2->menu(),
							&item->history()->session(),
							authorId
						));
					}
				}
			}

			if (containsSingleCustomEmojiPack) {
				auto authorId = getUserIdFromPackId(emojiPacks.front().id);

				if (authorId != 0) {
					menu2->addAction(Ui::ContextActionStickerAuthor(
						menu2->menu(),
						&item->history()->session(),
						authorId
					));
				}
			}
		},
	});
}


ID getUserIdFromPackId(uint64 id) {
	auto ownerId = id >> 32;
	if ((id >> 16 & 0xff) == 0x3f) {
		ownerId |= 0x80000000;
	}
	if (id >> 24 & 0xff) {
		ownerId += 0x100000000;
	}

	return ownerId;
}

// stole form ayugram
QString formatDateTime(const QDateTime &date) {
	const auto locale = QLocale::system();
	const auto datePart = locale.toString(date.date(), QLocale::ShortFormat);
	const auto timePart = locale.toString(date, "HH:mm:ss");

	return datePart + getLocalizedAt() + timePart;
}