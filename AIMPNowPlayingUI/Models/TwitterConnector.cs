using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Livet;

using CoreTweet;
using CoreTweet.Core;
using static CoreTweet.OAuth;
using AIMPNowPlayingUI.ViewModels;
using System.IO;

namespace AIMPNowPlayingUI.Models {
    public class TwitterConnector : NotificationObject {


        private OAuthSession RequestSession;


        private Tokens Token;

        private MainWindowViewModel Owner;

        public TwitterConnector(MainWindowViewModel vm) {

            Owner = vm;
            
        }

        public async void RequestToken() {

            RequestSession = await AuthorizeAsync(TwitterConstants.ConsumerKey, TwitterConstants.ConsumerSecret);
            System.Diagnostics.Process.Start(RequestSession.AuthorizeUri.OriginalString);
        }

        public async void Verify(string pin) {

            if(RequestSession != null) {

                Token = await RequestSession.GetTokensAsync(pin);
                Owner.AccessToken = Token.AccessToken;
                Owner.AccessTokenSecret = Token.AccessTokenSecret;
                Owner.CanTweet = true;
            }
        }


        public void Tweet(string text, string jacketPath) {

            if(Owner.CanTweet) {

                if (Token == null) {

                    Token = Tokens.Create(TwitterConstants.ConsumerKey, TwitterConstants.ConsumerSecret, Owner.AccessToken, Owner.AccessTokenSecret);
                }

                if(text.Length > 140) {

                    text = text.Substring(0, 139) + "…";
                }

                if(jacketPath != null) {

                    var result = Token.Media.Upload(new FileInfo(jacketPath));

                    Token.Statuses.Update(status: text, media_ids: new long[] { result.MediaId }, tweet_mode: TweetMode.extended);
                } else {

                    Token.Statuses.Update(text, tweet_mode: TweetMode.extended);
                }

            }

        }







    }
}
